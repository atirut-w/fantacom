#pragma once
#include <Z80.h>
#include <array>
#include <cstdint>
#include <vector>
#include <map>
#include <io_device.hpp>
#include <mutex>

class Machine
{
public:
    enum MMUFlags
    {
        ROM_ENABLE = 1 << 0,
    };
    
    Machine();

    Z80 cpu;
    std::array<uint8_t, 0x2000> rom;

    std::vector<uint8_t> ram;
    std::array<uint8_t, 16> pagetable;
    uint8_t mmu_flags;

    std::map<uint16_t, IODevice*> io_devices;

    std::mutex cpu_mutex;
    std::mutex ram_mutex; // Thought this wasn't needed, but multiple devices may access RAM at once
};
