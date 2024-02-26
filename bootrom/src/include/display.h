#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t attribute;
    uint8_t character;
} Character;

void display_init();
void printchar(const char c);
void print(const char *s);
