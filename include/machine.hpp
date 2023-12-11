#pragma once
#include <Z80.h>
#include <array>
#include <cstdint>
#include <vector>
#include <map>
#include <io_device.hpp>

class Machine
{
public:
    Machine();

    Z80 cpu;
    std::array<uint8_t, 0x2000> rom;
    std::vector<uint8_t> ram;
    std::array<uint8_t, 16> pagetable;

    std::map<uint16_t, IODevice*> io_devices;
};
