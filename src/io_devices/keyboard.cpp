#include <io_devices/keyboard.hpp>
#include <machine.hpp>
#include <raylib.h>

Keyboard::Keyboard(void *ctx) : IODevice(ctx)
{
    size = sizeof(registers);
}

uint8_t Keyboard::in(uint16_t addr)
{
    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void Keyboard::out(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}

void Keyboard::update()
{
    Machine &machine = *(Machine *)ctx;

    auto keycode = GetKeyPressed();
    if (keycode > 0)
    {
        held_keys.push_back(keycode);
        registers.scancode = keycode;
        machine.queue_interrupt(registers.interrupt);
    }
    else
    {
        for (int i = held_keys.size() - 1; i >= 0; i--)
        {
            if (IsKeyReleased(held_keys[i]))
            {
                registers.scancode = held_keys[i] | 0x8000;
                machine.queue_interrupt(registers.interrupt);
                held_keys.erase(held_keys.begin() + i);
            }
            else if (IsKeyPressedRepeat(held_keys[i]))
            {
                registers.scancode = held_keys[i];
                machine.queue_interrupt(registers.interrupt);
            }
        }
    }
}
