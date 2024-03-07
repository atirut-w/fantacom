#pragma once
#include <io_device.hpp>
#include <raylib.h>

class Graphics : public IODevice
{
public:
    Texture2D font;
    RenderTexture2D screen_rt;

    Graphics(void *ctx);
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;
    void init();
    void render();

    struct
    {
        uint8_t buffer_page;
    } registers;
};

struct Character
{
    uint8_t attributes;
    uint8_t character;
};
