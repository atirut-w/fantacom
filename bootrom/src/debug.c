#include <debug.h>
#include <z80io.h>

#define STATUS_PORT 0x0100

void debug_status(const char status)
{
    outb(0x0100, status);
}
