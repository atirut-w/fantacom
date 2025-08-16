#include "argparse/argparse.hpp"
#include "board.hpp"
#include <fstream>
#include <memory>

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

  while (1) {
    board.cpu.tick(1);
  }

  return 0;
}
