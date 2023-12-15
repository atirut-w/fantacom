#include <io_devices/disp_kb_controller.hpp>

DisplayKeyboardController::DisplayKeyboardController(void *ctx)
{
    this->ctx = ctx;
    size = 0; // WIP
    lsb_only = true;
}

DisplayKeyboardController::~DisplayKeyboardController()
{
    // WIP
}

uint8_t DisplayKeyboardController::read(uint16_t addr)
{
    return 0; // WIP
}

void DisplayKeyboardController::write(uint16_t addr, uint8_t val)
{
    // WIP
}
