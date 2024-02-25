#include <io_devices/mmu.hpp>
#include <iostream>
#include <iomanip>

using namespace std;

MMU::MMU()
{
    size = sizeof(registers);
    registers.flags |= ROM_ENABLE;
}

uint8_t MMU::in(uint16_t addr)
{
    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void MMU::out(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}

uint8_t MMU::read(uint16_t addr)
{
    if (addr < rom->size() && (registers.flags & ROM_ENABLE))
    {
        return (*rom)[addr];
    }

    int page = registers.pagetable[addr >> 12];
    int physical = (page << 12) | (addr & 0xfff);

    if (physical >= ram->size())
    {
        // cerr << "WARN: Out of bounds read at $" << hex << addr << " -> $" << physical << endl;
        return 0;
    }

    return (*ram)[physical];
}

void MMU::write(uint16_t addr, uint8_t val)
{
    int page = registers.pagetable[addr >> 12];
    int physical = (page << 12) | (addr & 0xfff);

    if (physical >= ram->size())
    {
        // cerr << "WARN: Out of bounds write at $" << hex << addr << " -> $" << physical << endl;
        return;
    }

    (*ram)[physical] = val;
}
