#pragma once
#include <io_device.hpp>
#include <cstdint>

class DisplayKeyboardController : public IODevice
{
public:
    DisplayKeyboardController(void *ctx);
    ~DisplayKeyboardController();
    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t val) override;
};
