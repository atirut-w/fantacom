#pragma once
#include <io_device.hpp>
#include <vector>

class Keyboard : public IODevice
{
public:
    Keyboard();
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;
    
    struct
    {
        uint16_t scancode;
    } registers;
};
