#include <io_devices/disp_kb_controller.hpp>
#include <raylib.h>
#include <globals.hpp>
#include <machine.hpp>

void display_main(DisplayKeyboardController *controller)
{
    Machine &machine = *(Machine *)controller->ctx;
    
    InitWindow(720, 400, "Fantacom");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        controller->self_mutex.lock();

        controller->self_mutex.unlock();
        EndDrawing();
    }

    Globals::running = false;
}

DisplayKeyboardController::DisplayKeyboardController(void *ctx)
{
    this->ctx = ctx;
    size = 0; // WIP
    lsb_only = true;

    raylib_thread = std::thread(display_main, this);
}

DisplayKeyboardController::~DisplayKeyboardController()
{
    raylib_thread.join();
}

uint8_t DisplayKeyboardController::read(uint16_t addr)
{
    return 0; // WIP
}

void DisplayKeyboardController::write(uint16_t addr, uint8_t val)
{
    self_mutex.lock();
    
    if (addr < sizeof(control))
    {
        ((uint8_t *)&control)[addr] = val;
    }

    self_mutex.unlock();
}
