#pragma once
#include <cstdint>
#include <functional>

class IODevice
{
public:
    void *ctx;
    uint8_t size;
    bool lsb_only;

    IODevice(void *ctx) : ctx(ctx) {};
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t val) = 0;
};
