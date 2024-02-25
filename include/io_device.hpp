#pragma once
#include <cstdint>

class IODevice
{
public:
    void *ctx;
    uint8_t size;

    // IODevice(void *ctx) : ctx(ctx) {};
    virtual uint8_t in(uint16_t addr) = 0;
    virtual void out(uint16_t addr, uint8_t val) = 0;
};
