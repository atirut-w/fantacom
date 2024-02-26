#include <display.h>
#include <z80io.h>

volatile Character *screen = (Character *)0x3000;

void display_init()
{
    out_port(0x0100, 0x30); // Set VGA buffer to 0x3000
    for (int i = 0; i < 80 * 25; i++)
    {
        screen[i].attribute = 0x07;
        screen[i].character = ' ';
    }
}
