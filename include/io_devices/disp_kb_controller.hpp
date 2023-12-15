#pragma once
#include <io_device.hpp>
#include <cstdint>
#include <thread>
#include <mutex>

class DisplayKeyboardController : public IODevice
{
private:
    std::thread raylib_thread;
    std::mutex self_mutex;
public:
    DisplayKeyboardController(void *ctx);
    ~DisplayKeyboardController();
    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t val) override;
};
