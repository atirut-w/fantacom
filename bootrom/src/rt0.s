    ; Not-C Runtime 0
    .text

    .global _start
    .type _start, @function
_start:
    di ; No one interrupts us

    ld bc, 2 ; Page 2 remap IO
    ld d, 0 ; Current bank
    ld hl, 2000h ; Testing area

1:
    out (c), d
    ld (hl), 55h
    ld a, (hl)
    cp 55h
    jr z, 2f

    inc d
    jr nz, 1b
    jp _hang

2:
    ld sp, __sp_init
    call _init_data
    call main
    jp _hang

    .type _init_data, @function
_init_data:
    ld hl, __data_load
    ld de, __data_start
    ld bc, __data_size
    ldir
    ret

    .type _hang, @function
_hang:
    jr _hang

    .data
hello:
    ; Raw VGA text mode data for "Hello, world!". White on black.
    .byte 0x07, 'H', 0x07, 'e', 0x07, 'l', 0x07, 'l', 0x07, 'o', 0x07, ',', 0x07, ' ', 0x07, 'w', 0x07, 'o', 0x07, 'r', 0x07, 'l', 0x07, 'd', 0x07, '!'
