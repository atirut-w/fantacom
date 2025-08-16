#include "argparse/argparse.hpp"
#include "board.hpp"
#include "raylib.h"
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

  InitWindow(640, 480, "Fantacom");
  SetTargetFPS(60);
  int clk_adjust = 0;

  while (!WindowShouldClose()) {
    float delta = GetFrameTime();
    int target = static_cast<int>(FREQ * delta);
    int ran = board.cpu.tick(target - clk_adjust);
    clk_adjust = ran - target;
    std::cout << "Target: " << target << ", Ran: " << ran << ", Clock Adjust: " << clk_adjust << std::endl;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
