#pragma once

#include "bus.hpp"
#include "z80.hpp"
#include <array>
#include <cstdint>
#include <vector>

class Board {
public:
  Z80 cpu;
  Bus<uint32_t> physical;
  std::array<uint8_t, 8192> rom;
  std::vector<uint8_t> ram;
  std::array<uint8_t, 16> pagetable;
  Bus<uint16_t> io;

  Board();

  uint32_t resolve_address(uint16_t address) const;
};
