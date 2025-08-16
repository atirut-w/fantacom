#include "z80.hpp"
#include "Z80.h"
#include <cstdint>

Z80Wrapper::Z80Wrapper() {
  impl.context = this;

  // Mandatory callbacks
  impl.fetch_opcode = [](void *self, uint16_t address) {
    return static_cast<Z80Wrapper*>(self)->fetch_opcode(address);
  };
  impl.fetch = [](void *self, uint16_t address) {
    return static_cast<Z80Wrapper*>(self)->fetch(address);
  };
  impl.read = [](void *self, uint16_t address) {
    return static_cast<Z80Wrapper*>(self)->read(address);
  };
  impl.write = [](void *self, uint16_t address, uint8_t value) {
    static_cast<Z80Wrapper*>(self)->write(address, value);
  };
  impl.in = [](void *self, uint16_t port) {
    return static_cast<Z80Wrapper*>(self)->in(port);
  };
  impl.out = [](void *self, uint16_t port, uint8_t value) {
    static_cast<Z80Wrapper*>(self)->out(port, value);
  };

  // Optional callbacks
  impl.halt = [](void *self, uint8_t value) {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->halt) {
      wrapper->halt(value);
    }
  };
  impl.nop = [](void *self, uint16_t address) -> uint8_t {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->nop) {
      return wrapper->nop(address);
    }
    return 0;
  };
  impl.nmia = [](void *self, uint16_t address) -> uint8_t {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->nmia) {
      return wrapper->nmia(address);
    }
    return 0;
  };
  impl.inta = [](void *self, uint16_t address) -> uint8_t {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->inta) {
      return wrapper->inta(address);
    }
    return 0;
  };
  impl.int_fetch = [](void *self, uint16_t address) -> uint8_t {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->int_fetch) {
      return wrapper->int_fetch(address);
    }
    return 0;
  };
  impl.ld_i_a = [](void *self) {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->ld_i_a) {
      wrapper->ld_i_a();
    }
  };
  impl.ld_r_a = [](void *self) {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->ld_r_a) {
      wrapper->ld_r_a();
    }
  };
  impl.reti = [](void *self) {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->reti) {
      wrapper->reti();
    }
  };
  impl.retn = [](void *self) {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->retn) {
      wrapper->retn();
    }
  };
  impl.hook = [](void *self, uint16_t address) -> uint8_t {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->hook) {
      return wrapper->hook(address);
    }
    return 0;
  };
  impl.illegal = [](void *self, uint8_t opcode) -> uint8_t {
    auto wrapper = static_cast<Z80Wrapper*>(self);
    if (wrapper->illegal) {
      return wrapper->illegal(opcode);
    }
    return 0;
  };
}

int Z80Wrapper::tick(int cycles) {
  return z80_execute(&impl, cycles);
}
