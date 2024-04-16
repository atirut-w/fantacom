#include <z80io.h>

char inb(int port) __naked
{
__asm
    pop de ; Return address
    pop bc ; Port
    
    push af ; Save flags
    in a, (c)
    ld h, 0
    ld l, a ; Return value in HL
    pop af ; Restore flags

    push bc ; Restore registers
    push de
    ret
__endasm;
}

void outb(int port, char value) __naked
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

int inw(int port)
{
    return inb(port) | inb(port + 1) << 8;
}

void outw(int port, int value)
{
    outb(port, value & 0xff);
    outb(port + 1, value >> 8);
}
