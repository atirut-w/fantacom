#include <keyboard.h>
#include <stdio.h>
#include <z80io.h>
#include <ivt.h>

#define KEYBOARD_INTERRUPT 0x00

int buffer[16];
int head = 0;
int tail = 0;

int pop_key()
{
    if (head == tail)
        return 0;
    
    int scancode = buffer[tail++];
    tail %= 16;
    
    return scancode;
}

// Keyboard interrupt handler
void keyboard_handler() __interrupt
{
    int scancode = inw(0x0202);

    buffer[head++] = scancode;
    head %= 16;
    if (head == tail)
        tail = (tail + 1) % 16;
}

void keyboard_install_interrupt()
{
    outb(0x0200, KEYBOARD_INTERRUPT);
    ivt[KEYBOARD_INTERRUPT] = keyboard_handler;
}
