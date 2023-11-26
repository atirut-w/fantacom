#include <iostream>
#include <argparse/argparse.hpp>

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
        exit(0);
    }

    return 0;
}
