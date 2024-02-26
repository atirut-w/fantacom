#include <z80io.h>

int in_port(int port) __naked
{
__asm
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
