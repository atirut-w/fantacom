#include <io_devices/disk_ctrl.hpp>
#include <machine.hpp>

DiskCtrl::DiskCtrl(void *ctx) : IODevice(ctx)
{
    size = sizeof(registers);
}

uint8_t DiskCtrl::in(uint16_t addr)
{
    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void DiskCtrl::out(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}
