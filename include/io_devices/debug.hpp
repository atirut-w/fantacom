#pragma once
#include <io_device.hpp>

class Debug : public IODevice
{
public:
    Debug();
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;

    struct Registers
    {
        uint8_t status;
    } registers;
};
