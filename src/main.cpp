#include <iostream>
#include <argparse/argparse.hpp>
#include <machine.hpp>
#include <fstream>
#include <filesystem>
#include <raylib.h>
#include <memory>

void cpu_thread(int frequency, Machine *machine)
{
    int waste = 0;

    std::cout << "CPU thread started" << std::endl;
    while (!WindowShouldClose())
    {
        auto current_time = std::chrono::high_resolution_clock::now();

        if (waste > 0)
        {
            waste--;
        }
        else
        {
            machine->mutex.lock();
            waste = z80_run(&machine->cpu, 1); // 1 instruction
            machine->mutex.unlock();
        }

        auto delta = std::chrono::high_resolution_clock::now() - current_time;
        std::this_thread::sleep_for(std::chrono::seconds((long)(1.0 / frequency)) - delta);
    }
    std::cout << "CPU thread stopped" << std::endl;
}

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
    std::thread cpu(cpu_thread, parser->get<int>("--frequency") * 1000000, &machine);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();

        SetWindowTitle(("Fantacom - " + std::to_string(GetFPS()) + " FPS").c_str());
    }

    std::cout << "Joining CPU thread..." << std::endl;
    cpu.join();
    std::cout << "CPU thread joined" << std::endl;

    CloseWindow();

    return 0;
}
