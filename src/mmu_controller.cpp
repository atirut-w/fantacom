#include <mmu_controller.hpp>
#include <machine.hpp>

MMUController::MMUController(void *ctx) : IODevice(ctx)
{
    size = 17;
    lsb_only = true;
}

uint8_t MMUController::read(uint16_t addr)
{
    return 0;
}

void MMUController::write(uint16_t addr, uint8_t val)
{
    Machine &machine = *(Machine *)ctx;

    if (addr == 0)
    {
        // TODO: Set flags
    }
    else
    {
        machine.pagetable[addr - 1] = val;
    }
}
