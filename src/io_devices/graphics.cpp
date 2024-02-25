#include <io_devices/graphics.hpp>

Graphics::Graphics()
{
    size = sizeof(registers);
}

uint8_t Graphics::in(uint16_t addr)
{
    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void Graphics::out(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}
