#include <io_devices/debug.hpp>
#include <iostream>
#include <iomanip>

Debug::Debug()
{
    size = sizeof(Registers);
}

uint8_t Debug::in(uint16_t addr)
{
    if (addr == offsetof(Registers, status))
    {
        return 0;
    }
    return 0;
}

void Debug::out(uint16_t addr, uint8_t val)
{
    if (addr == offsetof(Registers, status))
    {
        std::cout << "Debug status: $" << std::hex << std::setw(2) << std::setfill('0') << (int)val << std::endl;
    }
}
