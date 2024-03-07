#include <keyboard.h>
#include <stdio.h>
#include <z80io.h>

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
void z80_rst_08h() __critical __interrupt(0)
{
    int scancode = in_port(0x0200) | in_port(0x0201) << 8;

    buffer[head++] = scancode;
    head %= 16;
    if (head == tail)
        tail = (tail + 1) % 16;
}
