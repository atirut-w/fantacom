#pragma once
#include "Z80.h"
#include <cstdint>
#include <functional>

class Z80Wrapper {

public:
  using Read = std::function<uint8_t(uint16_t)>;
  using Write = std::function<void(uint16_t, uint8_t)>;
  using Halt = std::function<void(uint8_t)>;
  using Notify = std::function<void()>;
  using Illegal = std::function<uint8_t(uint8_t)>;

  Z80 impl;
  
  Read fetch_opcode;
  Read fetch;
  Read read;
  Write write;
  Read in;
  Write out;
  Halt halt;
  Read nop;
  Read nmia;
  Read inta;
  Read int_fetch;
  Notify ld_i_a;
  Notify ld_r_a;
  Notify reti;
  Notify retn;
  Read hook;
  Illegal illegal;


  Z80Wrapper();
  int tick(int cycles);
};
