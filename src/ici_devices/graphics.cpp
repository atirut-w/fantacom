#include <ici_devices/graphics.hpp>
#include <machine.hpp>
#include <raylib.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define CHARACTER_WIDTH 8
#define CHARACTER_HEIGHT 16

// Bog-standard VGA palette
Color palette[16] = {
    {0,0,0,255}, // Black
    {0,0,170,255}, // Blue
    {0,170,0,255}, // Green
    {0,170,170,255}, // Cyan
    {170,0,0,255}, // Red
    {170,0,170,255}, // Magenta
    {170,85,0,255}, // Brown
    {170,170,170,255}, // Light Gray
    {85,85,85,255}, // Dark Gray
    {85,85,255,255}, // Light Blue
    {85,255,85,255}, // Light Green
    {85,255,255,255}, // Light Cyan
    {255,85,85,255}, // Light Red
    {255,85,255,255}, // Light Magenta
    {255,255,85,255}, // Yellow
    {255,255,255,255} // White
};

Graphics::Graphics(void *ctx) : ICIDevice(ctx)
{
    
}

uint8_t Graphics::read(uint16_t addr)
{
    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void Graphics::write(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}

void Graphics::init()
{
    screen_rt = LoadRenderTexture(SCREEN_WIDTH * CHARACTER_WIDTH, SCREEN_HEIGHT * CHARACTER_HEIGHT);
}

void Graphics::render()
{
    Machine &machine = *(Machine *)ctx;
    std::array<Character, SCREEN_WIDTH * SCREEN_HEIGHT> screen;

    machine.mutex.lock();
    {
        uint16_t screen_offset = machine.graphics->registers.buffer_page << 8;
        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
        {
            screen[i].attributes = machine.mmu->read(screen_offset++);
            screen[i].character = machine.mmu->read(screen_offset++);
        }
    }
    machine.mutex.unlock();

    BeginTextureMode(screen_rt);
    ClearBackground(BLACK);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        Character character = screen[i];
        DrawRectangle((i % SCREEN_WIDTH) * 8, (i / SCREEN_WIDTH) * 16, 8, 16, palette[(character.attributes >> 4) & 0x07]);
        Rectangle char_rect = {
            0,
            (float)character.character * 16,
            8,
            16
        };
        Rectangle dst_rect = {
            (float)(i % SCREEN_WIDTH) * 8,
            (float)(i / SCREEN_WIDTH) * 16,
            8,
            16
        };
        DrawTexturePro(font, char_rect, dst_rect, {0, 0}, 0, palette[character.attributes & 0x0f]);
    }
    EndTextureMode();
}
