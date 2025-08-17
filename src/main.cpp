#include "argparse/argparse.hpp"
#include "board.hpp"
#include "raylib.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

#define FREQ 4000000 // 4 MHz

using argparse::ArgumentParser;

std::unique_ptr<const ArgumentParser> parse_args(int argc, char *argv[]) {
  auto parser = std::make_unique<ArgumentParser>("fantacom");

  parser->add_argument("-b", "--bios")
      .help("Path to the BIOS file")
      .default_value("bios.bin");

  parser->add_argument("-m", "--memory")
      .help("Amount of RAM in 4KiB pages")
      .default_value(4u)
      .scan<'u', unsigned int>();

  try {
    parser->parse_args(argc, argv);
    return parser;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << *parser;
    return nullptr;
  }
}

int main(int argc, char *argv[]) {
  auto args = parse_args(argc, argv);
  if (!args) {
    return 1;
  }

  Board board;

  std::ifstream bios_file(args->get<std::string>("--bios"), std::ios::binary);
  if (!bios_file) {
    std::cerr << "Failed to open BIOS file: " << args->get<std::string>("--bios") << std::endl;
    return 1;
  }

  bios_file.read(reinterpret_cast<char *>(board.rom.data()), board.rom.size());
  bios_file.close();

  board.ram.resize(args->get<unsigned int>("--memory") * 4096);
  std::fill(board.ram.begin(), board.ram.end(), 0b01010101);

  InitWindow(80 * 8, 25 * 16, "Fantacom");
  SetTargetFPS(60);
  int clk_adjust = 0;
  Image frame = GenImageColor(80 * 8, 25 * 16, BLACK);
  Texture2D texture = LoadTextureFromImage(frame);

  while (!WindowShouldClose()) {
    float delta = GetFrameTime();
    int target = static_cast<int>(FREQ * delta);
    int ran = board.cpu.tick(target - clk_adjust);
    clk_adjust = ran - target;

    const int CHAR_WIDTH = 8;
    const int CHAR_HEIGHT = 16;
    int vram_base = board.vram_start << 12;
    int vga_buffer = vram_base + CHAR_HEIGHT * 256;

    for (int row = 0; row < 25; ++row) {
      for (int col = 0; col < 80; ++col) {
        uint8_t ch = board.physical.read(vga_buffer++);
        uint8_t attr = board.physical.read(vga_buffer++);
        int glyph_base = vram_base + ch * CHAR_HEIGHT;
        int base_x = col * CHAR_WIDTH;
        int base_y = row * CHAR_HEIGHT;

        int fg_idx = attr & 0x0F;
        int bg_idx = (attr >> 4) & 0x0F;

        Color fg = {
          static_cast<unsigned char>((fg_idx & 0x04 ? 0xAA : 0x00) | (fg_idx & 0x01 ? 0x55 : 0x00)),
          static_cast<unsigned char>((fg_idx & 0x02 ? 0xAA : 0x00) | (fg_idx & 0x01 ? 0x55 : 0x00)),
          static_cast<unsigned char>((fg_idx & 0x08 ? 0xAA : 0x00) | (fg_idx & 0x01 ? 0x55 : 0x00)),
          255
        };

        Color bg = {
          static_cast<unsigned char>((bg_idx & 0x04 ? 0xAA : 0x00) | (bg_idx & 0x01 ? 0x55 : 0x00)),
          static_cast<unsigned char>((bg_idx & 0x02 ? 0xAA : 0x00) | (bg_idx & 0x01 ? 0x55 : 0x00)),
          static_cast<unsigned char>((bg_idx & 0x08 ? 0xAA : 0x00) | (bg_idx & 0x01 ? 0x55 : 0x00)),
          255
        };

        for (int y = 0; y < CHAR_HEIGHT; ++y) {
          uint8_t line = board.physical.read(glyph_base + y);
          for (int x = 0; x < CHAR_WIDTH; ++x) {
            Color color = (line & (1 << (7 - x))) ? fg : bg;
            ImageDrawPixel(&frame, base_x + x, base_y + y, color);
          }
        }
      }
    }

    UpdateTexture(texture, frame.data);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(texture, 0, 0, WHITE);
    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
