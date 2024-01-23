#include <iostream>
#include <argparse/argparse.hpp>
#include <machine.hpp>
#include <fstream>
#include <filesystem>
#include <raylib.h>

using namespace std;

int main(int argc, char *argv[])
{
    argparse::ArgumentParser parser("fantacom");
    parser.add_description("Fantasy Computer emulator");

    // Boot ROM image
    parser.add_argument("bootrom")
        .help("Boot ROM image");

    // How many banks of 4KiB RAM to allocate
    parser.add_argument("-r", "--ram")
        .help("Number of 4KiB RAM banks to allocate")
        .default_value(4)
        .scan<'i', int>();
    
    // Approx. CPU frequency in MHz
    parser.add_argument("-f", "--frequency")
        .help("CPU frequency in MHz")
        .default_value(1)
        .scan<'i', int>();

    try
    {
        parser.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << parser;
        exit(1);
    }

    Machine machine;
    machine.ram.resize(parser.get<int>("--ram") * 0x1000);

    {
        ifstream bootrom(parser.get<string>("bootrom"), ios::binary);

        if (!bootrom)
        {
            cerr << "Could not open boot ROM image" << endl;
            exit(1);
        }
        if (filesystem::file_size(parser.get<string>("bootrom")) > machine.rom.size())
        {
            cerr << "WARN: Boot ROM image too large, truncating" << endl;
        }

        bootrom.read((char *)machine.rom.data(), machine.rom.size());
    }

    InitWindow(640, 480, "Fantacom - Initialising...");
    SetTargetFPS(60);
    int frequency = parser.get<int>("--frequency");

    while (!WindowShouldClose())
    {
        z80_run(&machine.cpu, GetFrameTime() * frequency * 1000000);
        
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();

        SetWindowTitle(("Fantacom - Running at " + to_string(GetFPS()) + " FPS").c_str());
    }

    return 0;
}
