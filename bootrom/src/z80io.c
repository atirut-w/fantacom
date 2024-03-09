#include <z80io.h>

char inc_port(int port) __naked
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

void outc_port(int port, char value) __naked
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

int inw_port(int port)
{
    return inc_port(port) | inc_port(port + 1) << 8;
}

void outw_port(int port, int value)
{
    outc_port(port, value & 0xff);
    outc_port(port + 1, value >> 8);
}
