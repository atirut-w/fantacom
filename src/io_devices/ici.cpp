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
                switch (registers.address)
                {
                case 0:
                    registers.data = devices[registers.device_select]->vendor_id & 0xff;
                case 1:
                    registers.data = (devices[registers.device_select]->vendor_id >> 8) & 0xff;
                case 2:
                    registers.data = devices[registers.device_select]->device_id & 0xff;
                case 3:
                    registers.data = (devices[registers.device_select]->device_id >> 8) & 0xff;
                case 4:
                    registers.data = devices[registers.device_select]->device_type;
                case 5:
                case 6:
                case 7:
                    registers.data = 0;
                default:
                    registers.data = devices[registers.device_select]->read(registers.address - 8);
                }
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
            if (devices[registers.device_select] && registers.address >= 8)
            {
                devices[registers.device_select]->write(registers.address - 8, val);
            }
            registers.address += registers.auto_increment;
        }

        ((uint8_t *)&registers)[addr] = val;
    }
}
