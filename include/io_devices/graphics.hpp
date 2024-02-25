#pragma once
#include <io_device.hpp>
#include <raylib.h>

class Graphics : public IODevice
{
public:
    Graphics();
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;

    struct
    {
        uint8_t buffer_page;
    } registers;
};

struct Character
{
    uint8_t attributes;
    uint8_t character;
};
