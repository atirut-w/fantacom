#include <io_devices/disp_kb_controller.hpp>
#include <raylib.h>
#include <globals.hpp>
#include <machine.hpp>

void display_main(DisplayKeyboardController *controller)
{
    Machine &machine = *(Machine *)controller->ctx;
    
    InitWindow(16 * 80, 16 * 25, "Fantacom");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        controller->self_mutex.lock();
        ClearBackground(BLACK);

        int vram_offset = controller->control.vram_page * 0x1000;
        char character[2] = {0, 0};

        for (int y = 0; y < 25; y++)
        {
            for (int x = 0; x < 80; x++)
            {
                character[0] = vram_offset > machine.ram.size() ? ' ' : machine.ram[vram_offset++];
                DrawText(character, x * 16, y * 16, 16, WHITE);
            }
        }

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
    if (addr < sizeof(control))
    {
        return ((uint8_t *)&control)[addr];
    }
    return 0;
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
