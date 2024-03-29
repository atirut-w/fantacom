#include <io_devices/ici.hpp>

ICI::ICI(void *ctx) : IODevice(ctx)
{
    size = sizeof(registers);
}

uint8_t ICI::in(uint16_t addr)
{
    return 0;
}

void ICI::out(uint16_t addr, uint8_t val)
{
    return;
}
