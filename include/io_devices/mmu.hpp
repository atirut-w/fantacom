#pragma once
#include <io_device.hpp>
#include <cstdint>
#include <vector>
#include <array>

class MMU : public IODevice
{
public:
    MMU();
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);

    std::array<uint8_t, 0x2000> *rom;
    std::vector<uint8_t> *ram;

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
