#pragma once
#include <Z80.h>
#include <array>
#include <cstdint>
#include <vector>
#include <io_device.hpp>
#include <io_devices/mmu.hpp>
#include <map>
#include <mutex>
#include <io_devices/graphics.hpp>

class Machine
{
public:
    Z80 cpu;
    std::array<uint8_t, 0x2000> rom;
    std::vector<uint8_t> ram;

    std::map<uint16_t, IODevice *> io_devices;
    std::mutex mutex;

    MMU *mmu;
    Graphics *graphics;

    Machine();
    ~Machine();
    void tick();
};
