#pragma once
#include "chips/z80.h"
#include <cstdint>
#include <functional>

class Z80 {
  z80_t impl;
  uint64_t pins;

public:
  std::function<uint8_t(uint16_t)> read;
  std::function<void(uint16_t, uint8_t)> write;
  std::function<uint8_t(uint16_t)> in;
  std::function<void(uint16_t, uint8_t)> out;

  Z80();
  void tick();
};
