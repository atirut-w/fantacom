#include <iostream>
#include <argparse/argparse.hpp>
#include <machine.hpp>
#include <fstream>

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
        bootrom.read((char *)machine.rom.data(), machine.rom.size());
    }

    while (1)
    {
        z80_execute(&machine.cpu, 1);
    }

    return 0;
}
