#include "argparse/argparse.hpp"
#include <memory>

using argparse::ArgumentParser;

std::unique_ptr<const ArgumentParser> parse_args(int argc, char *argv[]) {
  auto parser = std::make_unique<ArgumentParser>("fantacom");

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

  return 0;
}
