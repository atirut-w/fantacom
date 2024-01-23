#include <machine.hpp>
#include <iostream>
#include <iomanip>
#include <io_devices/mmu_controller.hpp>

using namespace std;

uint8_t read(void *ctx, uint16_t addr)
{
    Machine &machine = *(Machine *)ctx;
    
    if (addr < machine.rom.size() && (machine.mmu_flags & Machine::ROM_ENABLE))
    {
        return machine.rom[addr];
    }

    // Resolve physical address
    int page = machine.pagetable[addr >> 12];
    int physical = (page << 12) | (addr & 0xfff);

    if (physical >= machine.ram.size())
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

uint8_t in(void *ctx, uint16_t port)
{
    Machine &machine = *(Machine *)ctx;
    
    for (auto &pair : machine.io_devices)
    {
        IODevice *device = pair.second;
        uint16_t end = pair.first + device->size;
        uint16_t actual_port = device->lsb_only ? port & 0xff : port;
        
        if (actual_port >= pair.first && actual_port < end)
        {
            return device->read(actual_port - pair.first);
        }
    }

    cerr << "WARN: Unhandled I/O read at $" << hex << port << endl;
    return 0;
}

void out(void *ctx, uint16_t port, uint8_t val)
{
    Machine &machine = *(Machine *)ctx;
    
    for (auto &pair : machine.io_devices)
    {
        IODevice *device = pair.second;
        uint16_t end = pair.first + device->size;
        uint16_t actual_port = device->lsb_only ? port & 0xff : port;
        
        if (actual_port >= pair.first && actual_port < end)
        {
            device->write(actual_port - pair.first, val);
            return;
        }
    }
}

Machine::Machine()
{
    cpu.context = this;

    cpu.fetch_opcode = read;
    cpu.fetch = read;
    cpu.read = read;
    cpu.write = write;
    cpu.in = in;
    cpu.out = out;

#ifdef _WIN32
    // Null out unused callbacks, because Windows can't be trusted to zero out the struct
    cpu.halt = nullptr;
    cpu.nmia = nullptr;
#endif

    mmu_flags = ROM_ENABLE;
    io_devices[0] = new MMUController(this);
    // io_devices[0x20] = new DisplayKeyboardController(this);
}

Machine::~Machine()
{
    for (auto &pair : io_devices)
    {
        delete pair.second;
    }
}
