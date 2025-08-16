#include "board.hpp"
#include <cstdint>

Board::Board() {
  // ROM
  physical.add_device({
    0x00000,
    [this](uint32_t address) -> uint8_t {
      if (address < rom.size()) {
        return rom[address];
      }
      return 0;
    },
    [](uint32_t address, uint8_t value) {}
  });
  // RAM
  physical.add_device({
    0x80000,
    [this](uint32_t address) -> uint8_t {
      if (address < ram.size()) {
        return ram[address];
      }
      return 0;
    },
    [this](uint32_t address, uint8_t value) {
      if (address < ram.size()) {
        ram[address] = value;
      }
    }
  });

  // MMU page table
  io.add_device({
    0x0000,
    [this](uint32_t address) -> uint8_t {
      if (address < pagetable.size()) {
        return pagetable[address];
      }
      return 0;
    },
    [this](uint32_t address, uint8_t value) {
      if (address < pagetable.size()) {
        pagetable[address] = value;
      }
    }
  });

  cpu.read = [this](uint16_t address) -> uint8_t {
    uint32_t resolved = resolve_address(address);
    return physical.read(resolved);
  };
  cpu.write = [this](uint16_t address, uint8_t value) {
    uint32_t resolved = resolve_address(address);
    physical.write(resolved, value);
  };
  cpu.in = [this](uint16_t port) -> uint8_t {
    return io.read(port);
  };
  cpu.out = [this](uint16_t port, uint8_t value) {
    io.write(port, value);
  };

  cpu.fetch_opcode = cpu.read;
  cpu.fetch = cpu.read;
}

uint32_t Board::resolve_address(uint16_t address) const {
  int vpage = address >> 12;
  int offset = address & 0xFFF;
  return (pagetable[vpage] << 12) | offset;
}
