#include <machine.hpp>
#include <iostream>
#include <iomanip>
#include <io_devices/mmu.hpp>
#include <memory>

using namespace std;

uint8_t read(void *ctx, uint16_t addr)
{
    return ((Machine *)ctx)->mmu->read(addr);
}

void write(void *ctx, uint16_t addr, uint8_t val)
{
    ((Machine *)ctx)->mmu->write(addr, val);
}

uint8_t in(void *ctx, uint16_t port)
{
    Machine &machine = *(Machine *)ctx;

    for (auto &pair : machine.io_devices)
    {
        auto device = pair.second;
        uint16_t end = pair.first + device->size;

        if (port >= pair.first && port < end)
        {
            return device->in(port - pair.first);
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
        auto device = pair.second;
        uint16_t end = pair.first + device->size;

        if (port >= pair.first && port < end)
        {
            device->out(port - pair.first, val);
            return;
        }
    }
}

Machine::Machine()
{
    std::memset(&cpu, 0, sizeof(cpu)); // TODO: Z80 core as a C++ library and not C
    cpu.context = this;

    cpu.fetch_opcode = read;
    cpu.fetch = read;
    cpu.read = read;
    cpu.write = write;
    cpu.in = in;
    cpu.out = out;

    mmu = std::make_shared<MMU>();
    mmu->rom = &rom;
    mmu->ram = &ram;
    io_devices[0] = mmu;

    graphics = std::make_shared<Graphics>();
    io_devices[0x0100] = graphics;
}
