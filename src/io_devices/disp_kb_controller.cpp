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
        ClearBackground(BLACK);
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
    raylib_thread.detach();
}

DisplayKeyboardController::~DisplayKeyboardController()
{
    raylib_thread.~thread();
}

uint8_t DisplayKeyboardController::read(uint16_t addr)
{
    return 0; // WIP
}

void DisplayKeyboardController::write(uint16_t addr, uint8_t val)
{
    // WIP
}
