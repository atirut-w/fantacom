#include <machine.hpp>
#include <iostream>
#include <iomanip>

using namespace std;

uint8_t read(void *ctx, uint16_t addr)
{
    Machine &machine = *(Machine *)ctx;

    // Resolve physical address
    int page = machine.pagetable[addr >> 12];
    int physical = (page << 12) | (addr & 0xfff);
    
    if (physical < machine.rom.size())
    {
        return machine.rom[physical];
    }
    else if (physical >= machine.ram.size())
    {
        cerr << "WARN: Out of bounds read at $" << hex << addr << " -> $" << physical << endl;
        return 0;
    }

    return machine.ram[physical];
}

void write(void *ctx, uint16_t addr, uint8_t val)
{
    Machine &machine = *(Machine *)ctx;

    // Resolve physical address
    int page = machine.pagetable[addr >> 12];
    int physical = (page << 12) | (addr & 0xfff);

    if (physical >= machine.ram.size())
    {
        cerr << "WARN: Out of bounds write at $" << hex << addr << " -> $" << physical << endl;
        return;
    }

    machine.ram[physical] = val;
}

Machine::Machine()
{
    cpu.context = this;

    cpu.fetch_opcode = read;
    cpu.fetch = read;
    cpu.read = read;
    cpu.write = write;

    pagetable[0] = 0;
}
