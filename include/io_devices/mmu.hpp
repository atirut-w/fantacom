#pragma once
#include <io_device.hpp>
#include <cstdint>

class MMU : public IODevice
{
public:
    MMU();
    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t val) override;

    enum Flags
    {
        ROM_ENABLE = 1 << 0,
    };

    struct
    {
        uint8_t flags;
        uint8_t pagetable[16];
    } registers;
};
