#include <z80io.h>

char in_port(int port) __naked
{
__asm
    pop de ; Return address
    pop bc ; Port
    
    push af ; Save flags
    in a, (c)
    ld h, 0
    ld l, a
    pop af ; Restore flags

    push bc ; Restore registers
    push de
    ret
__endasm;
}

void out_port(int port, char value) __naked
{
__asm
    pop de ; Return address
    pop hl ; Value to write
    pop bc ; Port
    out (c), l

    push bc ; Restore registers
    push hl
    push de
    ret
__endasm;
}
