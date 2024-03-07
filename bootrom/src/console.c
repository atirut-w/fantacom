#include <z80io.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    uint8_t attribute;
    uint8_t character;
} Character;

volatile Character *screen = (Character *)0x3000;
int cursor = 0;

void scroll_line()
{
    char *screen_ptr = (char *)0x3000;
    
    memcpy(screen_ptr, screen_ptr + (80 * 2), 80 * 24 * 2);
    memset(screen_ptr + (80 * 24 * 2), 0, 80 * 2);
}

int fputc_cons_native(char c)
{
    screen[cursor].attribute = 0x07;
    switch (c)
    {
    default:
        screen[cursor].character = c;

        if (cursor + 1 >= 80 * 25)
        {
            scroll_line();
            cursor -= cursor % 80;
        }
        else
        {
            cursor++;
        }
        break;
    case '\b':
        if (cursor > 0)
            cursor--;
        break;
    case '\n':
        if (cursor + 80 >= 80 * 25)
            scroll_line();
        else
            cursor += 80;

        // Fall-through to `\r` to start at beginning of line
    case '\r':
        cursor -= cursor % 80;
        break;
    }
    screen[cursor].attribute = 0x70;
    return c;
}
