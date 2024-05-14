#pragma once
#include <io_device.hpp>
#include <ici_device.hpp>
#include <array>
#include <memory>
#include <cstdint>

// Internal Component Interconnect
class ICI : public IODevice
{
public:
    std::array<std::shared_ptr<ICIDevice>, 256> devices;

    ICI(void *ctx);
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;

    struct Registers
    {
        uint8_t auto_increment;
        uint8_t device_select;
        uint8_t address;
        uint8_t data;
    } registers;
};
