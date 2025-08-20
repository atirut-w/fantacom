#include "board.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>

Board::Board() {
  display.physical = &physical;
  mmu.physical = &physical;
  mmu.io = &io;

  // MMU configuration
  io.devices.push_back({
    0x0000, 0x0010,
    [this](uint16_t address) -> uint8_t {
      return mmu.pagetable[address];
    },
    [this](uint16_t address, uint8_t value) {
      mmu.pagetable[address] = value;
    }
  });

  // VGA configuration
  io.devices.push_back({
    0x0200, 0x0001,
    [this](uint16_t address) -> uint8_t {
      return display.mode;
    },
    [this](uint16_t address, uint8_t value) {
      display.mode = value;
    }
  });
  io.devices.push_back({
    0x0201, 0x0004,
    [this](uint16_t address) -> uint8_t {
      return display.plane_pages[address];
    },
    [this](uint16_t address, uint8_t value) {
      display.plane_pages[address] = value;
    }
  });

  // Debug console
  io.devices.push_back({
    0x0300, 0x0001,
    [this](uint16_t address) -> uint8_t {
      int val = std::cin.get();
      return val == EOF ? 0 : val;
    },
    [this](uint16_t address, uint8_t value) {
      std::cout << value;
    }
  });

  cpu.read = [this](uint16_t address) -> uint8_t {
    return mmu.read(address);
  };
  cpu.write = [this](uint16_t address, uint8_t value) {
    mmu.write(address, value);
  };
  cpu.in = [this](uint16_t port) -> uint8_t {
    return mmu.in(port);
  };
  cpu.out = [this](uint16_t port, uint8_t value) {
    mmu.out(port, value);
  };

  cpu.fetch_opcode = cpu.read;
  cpu.fetch = cpu.read;
}
