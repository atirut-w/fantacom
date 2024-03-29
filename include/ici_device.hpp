#pragma once
#include <cstdint>

class ICIDevice
{
public:
    void *ctx;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t device_type;

    ICIDevice() : ctx(nullptr) {}
    ICIDevice(void *ctx) : ctx(ctx) {};
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t val) = 0;
};
