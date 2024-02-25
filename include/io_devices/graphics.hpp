#pragma once
#include <io_device.hpp>

class Graphics : public IODevice
{
public:
    Graphics();
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;

    struct
    {
        uint8_t txt_page;
    } registers;
};
