#include <io_devices/keyboard.hpp>

Keyboard::Keyboard(void *ctx) : IODevice(ctx)
{
    size = sizeof(registers);
}

uint8_t Keyboard::in(uint16_t addr)
{
    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void Keyboard::out(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}
