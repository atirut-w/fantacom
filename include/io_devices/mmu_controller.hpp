#pragma once
#include <io_device.hpp>
#include <cstdint>

class MMUController : public IODevice
{
public:
    MMUController(void *ctx);
    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t val) override;
};
