#include <io_devices/ici.hpp>

ICI::ICI(void *ctx) : IODevice(ctx)
{
    size = sizeof(registers);
}

uint8_t ICI::in(uint16_t addr)
{
    if (addr < sizeof(registers))
    {
        if (addr == offsetof(Registers, data))
        {
            if (devices[registers.device_select])
            {
                registers.data = devices[registers.device_select]->read(registers.address);
            }
            else
            {
                registers.data = 0xff;
            }
            registers.address += registers.auto_increment;
        }

        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void ICI::out(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        if (addr == offsetof(Registers, data))
        {
            if (devices[registers.device_select])
            {
                devices[registers.device_select]->write(registers.address, val);
            }
            registers.address += registers.auto_increment;
        }

        ((uint8_t *)&registers)[addr] = val;
    }
}
