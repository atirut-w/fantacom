# MMU programming
Fantacom's selling point is that it has an MMU that allows handling of memory up to 1MiB. Neat, but that's not the reason this section is up top.

When Fantacom starts up, the memory map will be uninitialized, so god knows where each banks in logical address space map to in physical address space.

First off, you'll want to take a look at the MMU's I/O registers. They reside at `$0000` in I/O space:

| Offset  |     Type      | Description |
| :-----: | :-----------: | :---------: |
| `$0000` | `uint8_t[16]` | Remap table |
| `$0010` |   `uint8_t`   |    Flags    |

See the remap table? You will need to configure it to be able to use RAM. The 64KiB logical space is divided into 16 4KiB banks, and the table controls what physical bank each of the 64KiB address space is mapped to.

Here is an example code for finding an available bank:

```x86asm
    ld bc, 2 ; Page 2 remap register
    ld d, 0 ; Current bank we're testing
    ld hl, 0x2000 ; Address to test on

memcheck_loop:
    out (c), d

    ld (hl), 0x55
    ld a, (hl)
    cp 0x55
    jr z, memcheck_ok

    inc d
    jr nz, memcheck_loop
nomem:
    halt ; No point in continuing, we're toast
    jr nomem

memcheck_ok:
```

In this example, we're looping through each bank and checking if we can write to it (we're testing at `$2000`, hence page 2). If we can, we simply exit the loop. If we cannot write, we increment the bank. If it overflows... well, not like you can do anything without RAM. If it doesn't, we continue checking until we either find a bank or overflow.

As for the flag, its only purpose right now is to unmap the ROM out so that you can use the lower 8KiB in logical address space.
