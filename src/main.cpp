#include <iostream>
#include <argparse/argparse.hpp>
#include <machine.hpp>
#include <fstream>
#include <filesystem>
#include <raylib.h>
#include <memory>

std::shared_ptr<const argparse::ArgumentParser> parse_args(int argc, char *argv[])
{
    auto parser = std::make_shared<argparse::ArgumentParser>("fantacom");
    parser->add_description("Fantasy Computer emulator");

    // Boot ROM image
    parser->add_argument("bootrom")
        .help("Boot ROM image");
    
    // How many banks of 4KiB RAM to allocate
    parser->add_argument("-r", "--ram")
        .help("Number of 4KiB RAM banks to allocate")
        .default_value(4)
        .scan<'i', int>();
    
    // Target CPU frequency in MHz
    parser->add_argument("-f", "--frequency")
        .help("Target CPU frequency in MHz")
        .default_value(1)
        .scan<'i', int>();
    
    try
    {
        parser->parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << *parser;
        exit(1);
    }

    return parser;
}

int main(int argc, char *argv[])
{
    auto parser = parse_args(argc, argv);

    Machine machine;
    machine.ram.resize(parser->get<int>("--ram") * 0x1000);

    {
        std::ifstream bootrom(parser->get<std::string>("bootrom"), std::ios::binary);

        if (!bootrom)
        {
            std::cerr << "Could not open boot ROM image" << std::endl;
            exit(1);
        }
        if (std::filesystem::file_size(parser->get<std::string>("bootrom")) > machine.rom.size())
        {
            std::cerr << "WARN: Boot ROM image too large, truncating" << std::endl;
        }

        bootrom.read((char *)machine.rom.data(), machine.rom.size());
    }

    InitWindow(640, 480, "Fantacom - Initialising...");
    SetTargetFPS(60);
    int frequency = parser->get<int>("--frequency");
    int runover = 0; // The CPU is instruction-stepped, so we need to keep track of how many cycles we've run over by

    while (!WindowShouldClose())
    {
        int target = frequency * 1000000 * GetFrameTime();
        int cycles_ran = z80_run(&machine.cpu, target - runover);
        runover = cycles_ran - target;
        
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();

        SetWindowTitle(("Fantacom - " + std::to_string(GetFPS()) + " FPS - " + std::to_string(cycles_ran / GetFrameTime() / 1000000) + " MHz").c_str());
    }

    return 0;
}
