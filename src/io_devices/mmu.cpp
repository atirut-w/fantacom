#include <io_devices/mmu.hpp>
#include <machine.hpp>

MMU::MMU()
{
    size = sizeof(registers);
    lsb_only = true;

    registers.flags |= ROM_ENABLE;
}

uint8_t MMU::read(uint16_t addr)
{
    Machine &machine = *(Machine *)ctx;

    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void MMU::write(uint16_t addr, uint8_t val)
{
    Machine &machine = *(Machine *)ctx;

    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}
