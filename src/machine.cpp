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
        IODevice *device = pair.second;
        uint16_t end = pair.first + device->size;
        uint16_t actual_port = device->lsb_only ? port & 0xff : port;

        if (actual_port >= pair.first && actual_port < end)
        {
            return device->in(actual_port - pair.first);
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
            device->out(actual_port - pair.first, val);
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

    mmu = new MMU();
    mmu->rom = &rom;
    mmu->ram = &ram;

    io_devices[0] = mmu;
    // io_devices[0x20] = new DisplayKeyboardController(this);
}

Machine::~Machine()
{
    for (auto &pair : io_devices)
    {
        delete pair.second;
    }
}
