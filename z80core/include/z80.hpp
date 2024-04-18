#pragma once
#include <functional>
#include <cstdint>

typedef std::function<uint8_t(void *, uint16_t)> Z80Read;
typedef std::function<void(void *, uint16_t, uint8_t)> Z80Write;

class Z80
{
public:
    void *context;
    
    size_t run(size_t cycles);
    void interrupt(bool state);
    void nmi();

    // Callbacks

    Z80Read fetch_opcode;
    Z80Read fetch;
    Z80Read read;
    Z80Write write;
    Z80Read in;
    Z80Write out;
    Z80Read inta;
    Z80Read int_fetch;
};
