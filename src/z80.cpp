#define CHIPS_IMPL

#include "chips/z80.h"
#include "z80.hpp"

Z80::Z80() { pins = z80_init(&impl); }

void Z80::tick() {
  pins = z80_tick(&impl, pins);

  if (pins & Z80_PIN_MREQ) {
    uint16_t address = Z80_GET_ADDR(pins);
    if (pins & Z80_PIN_RD) {
      Z80_SET_DATA(pins, read(address));
    } else if (pins & Z80_PIN_WR) {
      write(address, Z80_GET_DATA(pins));
    }
  } else if (pins & Z80_PIN_IORQ) {
    uint16_t address = Z80_GET_ADDR(pins);
    if (pins & Z80_PIN_RD) {
      Z80_SET_DATA(pins, in(address));
    } else if (pins & Z80_PIN_WR) {
      out(address, Z80_GET_DATA(pins));
    }
  }
}
