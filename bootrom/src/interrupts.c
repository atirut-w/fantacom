#include <stdio.h>
#include <z80io.h>

// Keyboard interrupt handler
void z80_rst_08h() __critical __interrupt(0)
{
    int scancode = in_port(0x0200);
    printf("Scancode 0x%02x pressed\n", scancode);
}
