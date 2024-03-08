#pragma once
#include <io_device.hpp>
#include <vector>

class Keyboard : public IODevice
{
public:
    std::vector<int> held_keys;

    Keyboard(void *ctx);
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;
    void update();
    
    struct
    {
        uint8_t interrupt;
        uint16_t scancode;
    } registers;
};
