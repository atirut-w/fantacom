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

int fputc_cons_native(char c)
{
    switch (c)
    {
    default:
        screen[cursor].character = c;
        screen[cursor].attribute = 0x07;
        cursor++;
        break;
    case '\r':
        cursor -= cursor % 80;
        break;
    case '\n':
        cursor += 80;
        break;
    }
}
