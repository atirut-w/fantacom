#include <machine.hpp>
#include <iostream>
#include <iomanip>
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

uint8_t int_ack(void *ctx, uint16_t address)
{
    Machine &machine = *(Machine *)ctx;
    z80_int(&machine.cpu, 0);
    
    if (machine.interrupt_queue.size() > 0)
    {
        auto value = machine.interrupt_queue[0];
        machine.interrupt_queue.erase(machine.interrupt_queue.begin());
        return value << 1;
    }
    else
    {
        return 0;
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
    cpu.inta = int_ack;
    cpu.int_fetch = int_ack;

    mmu = std::make_shared<MMU>(this);
    mmu->rom = &rom;
    mmu->ram = &ram;
    io_devices[0] = mmu;

    ici = std::make_shared<ICI>(this);
    io_devices[0x100] = ici;

    graphics = std::make_shared<Graphics>(this);
    // io_devices[0x0100] = graphics;

    keyboard = std::make_shared<Keyboard>(this);
    // io_devices[0x0200] = keyboard;

    disk_ctrl = std::make_shared<DiskCtrl>(this);
    // io_devices[0x0300] = disk_ctrl;
}

void Machine::queue_interrupt(uint8_t interrupt)
{
    if (interrupt > 128)
    {
        std::cout << "WARN: Bad interrupt vector (0x" << std::hex << (int)interrupt << "), not queueing" << std::endl;
        return;
    }
    
    mutex.lock();
    interrupt_queue.push_back(interrupt);
    mutex.unlock();
}

void Machine::nmi_interrupt()
{
    z80_nmi(&cpu);
}

int Machine::run(int cycles)
{
    mutex.lock();
    if (interrupt_queue.size() > 0)
        z80_int(&cpu, 1);
    int actual_cycles = z80_run(&cpu, cycles);
    mutex.unlock();
    return actual_cycles;
}
