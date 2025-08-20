#pragma once

#include "bus.hpp"
#include "raylib.h"
#include "z80.hpp"
#include <array>
#include <cstdint>
#include <sys/types.h>
#include <vector>

struct Display {
  enum Mode {
    TEXT,
    GFX320x200x16,
    GFX320x200x256,
  };

  Bus<uint32_t> *physical;
  uint8_t mode = TEXT;
  std::array<uint8_t, 4> plane_pages;
  Image image;
  Texture2D texture;

  void init() {
    update_mode();
  }

  void update_mode() {
    UnloadImage(image);
    UnloadTexture(texture);
    switch (mode) {
      case TEXT:
        image = GenImageColor(80 * 8, 25 * 16, BLACK);
        break;
      case GFX320x200x16:
      case GFX320x200x256:
        image = GenImageColor(320, 200, BLACK);
        break;
    }
    texture = LoadTextureFromImage(image);
  }

  Color deindex_color(int idx) {
    return Color{
      static_cast<unsigned char>((idx & 0x04 ? 0xAA : 0x00) | (idx & 0x01 ? 0x55 : 0x00)),
      static_cast<unsigned char>((idx & 0x02 ? 0xAA : 0x00) | (idx & 0x01 ? 0x55 : 0x00)),
      static_cast<unsigned char>((idx & 0x08 ? 0xAA : 0x00) | (idx & 0x01 ? 0x55 : 0x00)),
      255
    };
  }

  Color get_pixel(int idx) {
    uint32_t p0 = plane_pages[0] << 12;
    uint32_t p1 = plane_pages[1] << 12;
    uint32_t p2 = plane_pages[2] << 12;
    uint32_t p3 = plane_pages[3] << 12;

    int byte_offset = idx >> 3;
    int bit_offset = 7 - (idx & 7);
    
    uint8_t b0 = physical->read(p0 + byte_offset);
    uint8_t b1 = physical->read(p1 + byte_offset);
    uint8_t b2 = physical->read(p2 + byte_offset);
    uint8_t b3 = physical->read(p3 + byte_offset);

    uint8_t color = 0;
    color |= ((b0 >> bit_offset) & 1) << 0; // Plane 0
    color |= ((b1 >> bit_offset) & 1) << 1; // Plane 1
    color |= ((b2 >> bit_offset) & 1) << 2; // Plane 2
    color |= ((b3 >> bit_offset) & 1) << 3; // Plane 3
    return deindex_color(color);
  }

  void draw() {
    switch (mode) {
      case TEXT: {
        const int CHAR_WIDTH = 8;
        const int CHAR_HEIGHT = 16;
        int buffer = plane_pages[0] << 12;
        int charset_base = plane_pages[1] << 12;

        for (int row = 0; row < 25; ++row) {
          for (int col = 0; col < 80; ++col) {
            uint8_t ch = physical->read(buffer++);
            uint8_t attr = physical->read(buffer++);
            int glyph_base = charset_base + ch * CHAR_HEIGHT;
            int base_x = col * CHAR_WIDTH;
            int base_y = row * CHAR_HEIGHT;

            int fg_idx = attr & 0x0F;
            int bg_idx = (attr >> 4) & 0x0F;

            Color fg = deindex_color(fg_idx);
            Color bg = deindex_color(bg_idx);

            for (int y = 0; y < CHAR_HEIGHT; ++y) {
              uint8_t line = physical->read(glyph_base + y);
              for (int x = 0; x < CHAR_WIDTH; ++x) {
                Color color = (line & (1 << (7 - x))) ? fg : bg;
                ImageDrawPixel(&image, base_x + x, base_y + y, color);
              }
            }
          }
        }

        break;
      }
    case GFX320x200x16: {
      for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 200; y++) {
          Color color = get_pixel(x + y * 320);
          ImageDrawPixel(&image, x, y, color);
        }
      }
      break;
    }
    case GFX320x200x256: {
      int ptr = plane_pages[0] << 12;
      for (int y = 0; y < 200; y++) {
        for (int x = 0; x < 320; x++) {
          uint8_t idx = physical->read(ptr++);
          Color color;

          if (idx < 16) {
            color = deindex_color(idx);
          } else if (idx < 232) {
            uint8_t val = idx - 16;
            const uint8_t levels[6] = { 0, 51, 82, 115, 165, 222 };
            color = {
              levels[(val / 36)],
              levels[(val % 36) / 6],
              levels[val % 6],
              255
            };
          } else {
            uint8_t grey = (idx - 232) * 10 + 8;
            color = { grey, grey, grey, 255 };
          }

          ImageDrawPixel(&image, x, y, color);
        }
      }
      break;
    }
    }

    UpdateTexture(texture, image.data);
  }
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
