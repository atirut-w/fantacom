#include <display.h>
#include <z80io.h>
#include <stdint.h>

volatile Character *screen = (Character *)0x3000;
int cursor = 0;

int display_init()
{
    // Find a memory bank for our VGA buffer
    // NOTE: We can continue on from the bank we used as RAM
    for (uint8_t i = in_port(2) + 1; i != 0; i++)
    {
        screen[0].attribute = 0x55;
        if (screen[0].attribute == 0x55)
            break;
        out_port(3, i);
    }
    if (in_port(3) == 0)
        return -1; // Checked all banks, no luck

    out_port(0x0100, 0x30); // Set VGA buffer to 0x3000
    for (int i = 0; i < 80 * 25; i++)
    {
        screen[i].attribute = 0x07;
        screen[i].character = 0;
    }

    return 0;
}

void putchar(const char c)
{
    switch (c)
    {
    default:
        screen[cursor].character = c;
        screen[cursor].attribute = 0x07;
        cursor++;
        break;
    }
}

void print(const char *s)
{
    while (*s)
        putchar(*s++);
}
