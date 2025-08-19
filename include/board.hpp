#pragma once

#include "bus.hpp"
#include "z80.hpp"
#include <array>
#include <cstdint>
#include <vector>

struct Display {
  uint8_t vram_start = 0;
  Bus<uint32_t> *physical;
};

struct PhysicalBus : public Bus<uint32_t> {
  std::array<uint8_t, 8192> rom;
  std::vector<uint8_t> ram;

  uint8_t read(uint32_t address) const override {
    if (address < rom.size()) {
      return rom[address];
    }
    if (address >= 0x80000 && address < 0x80000 + ram.size()) {
      return ram[address - 0x80000];
    }
    return 0;
  }

  void write(uint32_t address, uint8_t value) override {
    if (address >= 0x80000 && address < 0x80000 + ram.size()) {
      ram[address - 0x80000] = value;
    }
  }
};

struct MMU : public Bus<uint16_t> {
  std::array<uint8_t, 16> pagetable;
  Bus<uint32_t> *physical;
  Bus<uint16_t> *io;

  uint32_t resolve_address(uint16_t address) const {
    uint8_t page = pagetable[address >> 12];
    return (page << 12) | (address & 0x0fff);
  }

  uint8_t read(uint16_t address) const override {
    return physical->read(resolve_address(address));
  }

  void write(uint16_t address, uint8_t value) override {
    physical->write(resolve_address(address), value);
  }

  uint8_t in(uint16_t address) const {
    return io->read(address);
  }

  void out(uint16_t address, uint8_t value) {
    io->write(address, value);
  }
};

class Board {
public:
  Z80Wrapper cpu;
  Display display;
  PhysicalBus physical;
  DeviceBus<uint16_t> io;
  MMU mmu;

  Board();
};
