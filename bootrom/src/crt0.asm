    org 0x0000
    EXTERN _main

start:
    ; Initial RAM check
    ld bc, 0x2 ; MMU controller register for VM bank 3 remap
    ld d, 0 ; Start at bank 0
    ld e, 2 ; We want 2 banks minimum
    ld hl, 0x2000 ; Pointer to bank 3 in virtual memory

start_check_bank:
    out (c), d

    ld (hl), 0x55
    ld a, (hl)
    cp 0x55
    jr z, start_check_more

    inc d
    jr nz, start_check_bank
    jr nomem

start_check_more:
    ld a, e
    or a
    jr z, start_check_end
    inc c
    inc d
    dec e
    ld a, h
    add a, 0x10
    ld h, a
    jr start_check_bank

start_check_end:
    ld hl, 0x2fff
    ld sp, hl
    call _main
die:
    halt
    jr die

nomem:
    halt ; No point in continuing, we're toast.
    jr nomem
