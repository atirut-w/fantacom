#include <stdio.h>
#include <z80io.h>

// Keyboard interrupt handler
void z80_rst_08h() __critical __interrupt(0)
{
    int scancode = in_port(0x0200) | in_port(0x0201) << 8;
    if ((scancode & 0x8000) == 0)
    {
        if (scancode < 256)
        {
            putchar(scancode);
        }
        else
        {
            // Convert known keycodes to C0
            switch (scancode)
            {
            case 257:
                putchar('\n');
                break;
            case 259:
                putchar('\b');
                break;
            }
        }
    }
}
