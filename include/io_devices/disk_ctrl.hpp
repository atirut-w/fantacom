#pragma once
#include <io_device.hpp>

class DiskCtrl : public IODevice
{
public:
    DiskCtrl(void *ctx);
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;

    struct
    {

    } registers;
};
