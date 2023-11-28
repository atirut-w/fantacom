#include <machine.hpp>

uint8_t read(void *ctx, uint16_t addr)
{
    return 0;
}

void write(void *ctx, uint16_t addr, uint8_t val)
{

}

Machine::Machine()
{
    cpu.context = this;
    cpu.read = read;
    cpu.write = write;

    z80_power(&cpu, 1);
}
