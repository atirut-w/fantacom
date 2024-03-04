#include <z80io.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t attribute;
    uint8_t character;
} Character;

volatile Character *screen = (Character *)0x3000;
int cursor = 0;

void scroll_line() __naked
{
__asm
    ld bc, 80 * 24 * 2
    ld de, 0x3000
    ld hl, 0x3000 + (80 * 2)
    ldir

    ld bc, 80 * 2
    ld de, 0x3000 + (80 * 24 * 2) + 1
    ld hl, 0x3000 + (80 * 24 * 2)
    ld (hl), 0
    ldir

    ret
__endasm;
}

int fputc_cons_native(char c)
{
    switch (c)
    {
    default:
        screen[cursor].character = c;
        screen[cursor].attribute = 0x07;

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
    case '\n':
        if (cursor + 80 >= 80 * 25)
            scroll_line();
        else
            cursor += 80;
    case '\r':
        cursor -= cursor % 80;
        break;
    }
}
