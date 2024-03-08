#pragma once
#include <Z80.h>
#include <array>
#include <cstdint>
#include <vector>
#include <io_device.hpp>
#include <io_devices/mmu.hpp>
#include <map>
#include <mutex>
#include <io_devices/graphics.hpp>
#include <memory>
#include <io_devices/keyboard.hpp>

class Machine
{
public:
    Z80 cpu;
    std::vector<uint8_t> interrupt_queue;
    std::array<uint8_t, 0x2000> rom;
    std::vector<uint8_t> ram;

    std::map<uint16_t, std::shared_ptr<IODevice>> io_devices;
    std::mutex mutex;

    std::shared_ptr<MMU> mmu;
    std::shared_ptr<Graphics> graphics;
    std::shared_ptr<Keyboard> keyboard;

    Machine();
    void queue_interrupt(uint8_t interrupt);
    void nmi_interrupt();
    int tick();
};
