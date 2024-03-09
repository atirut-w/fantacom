#include <iostream>
#include <argparse/argparse.hpp>
#include <machine.hpp>
#include <fstream>
#include <filesystem>
#include <raylib.h>
#include <memory>
#include <atomic>
#include <unifont.hpp>
#include <cstdio>

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
        .default_value(1.0f)
        .scan<'g', float>();
    
    // Disk images
    parser->add_argument("-d", "--disk")
        .help("Disk images")
        .default_value(std::vector<std::string>())
        .nargs(argparse::nargs_pattern::at_least_one);

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

Texture2D generate_font_texture(const Unifont::Font &font)
{
    RenderTexture2D rt = LoadRenderTexture(8 * 16, 16 * 16);
    std::array<int, 256> cp437 = {
        0x0000, 0x263a, 0x263b, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022, 0x25d8, 0x25cb, 0x25d9, 0x2642, 0x2640, 0x266a, 0x266b, 0x263c,
        0x25ba, 0x25c4, 0x2195, 0x203c, 0x00b6, 0x00a7, 0x25ac, 0x21a8, 0x2191, 0x2193, 0x2192, 0x2190, 0x221f, 0x2194, 0x25b2, 0x25bc,
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
        0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
        0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
        0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
        0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x2302,

        0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7, 0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
        0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9, 0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
        0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba, 0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
        0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
        0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f, 0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
        0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b, 0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
        0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4, 0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
        0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248, 0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0,
    };

    BeginTextureMode(rt);
    ClearBackground(BLANK);
    for (int i = 0; i < 256; i++)
    {
        auto glyph = font.glyphs.at(i);
        int base_x = (i & 0x0f) * 8;
        int base_y = (i >> 4) * 16;
        for (int y = 0; y < 16; y++)
        {
            auto row = glyph.data[y];
            for (int x = 0; x < 8; x++)
            {
                if (row & (1 << x))
                {
                    DrawPixel(base_x + (7 - x), base_y + y, WHITE);
                }
            }
        }
    }
    EndTextureMode();

    Image img = LoadImageFromTexture(rt.texture);
    UnloadRenderTexture(rt);
    ImageFlipVertical(&img);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

int main(int argc, char *argv[])
{
    auto parser = parse_args(argc, argv);

    std::shared_ptr<Machine> machine = std::make_shared<Machine>();
    machine->ram.resize(parser->get<int>("--ram") * 0x1000);
    for (int i = 0; i < machine->ram.size(); i++)
    {
        machine->ram[i] = i & 1 ? '@' : 0x19;
    }

    {
        std::ifstream bootrom(parser->get<std::string>("bootrom"), std::ios::binary);

        if (!bootrom)
        {
            std::cerr << "Could not open boot ROM image" << std::endl;
            exit(1);
        }
        if (std::filesystem::file_size(parser->get<std::string>("bootrom")) > machine->rom.size())
        {
            std::cerr << "WARN: Boot ROM image too large, truncating" << std::endl;
        }

        bootrom.read((char *)machine->rom.data(), machine->rom.size());
    }

    for (auto filename : parser->get<std::vector<std::string>>("disk"))
    {
        auto stream = std::make_shared<std::ifstream>(filename, std::ios::binary);
        if (!*stream)
        {
            std::cout << "Unable to open disk image `" << filename << "`" << std::endl;
            exit(1);
        }
        machine->disk_ctrl->disks.push_back(Disk(stream));
    }

    std::ifstream font_file("font.hex");
    if (!font_file)
        throw std::runtime_error("Failed to open font file");
    Unifont::Font font(font_file);

    InitWindow(1280, 800, "Fantacom - Initializing...");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    machine->graphics->init();
    machine->graphics->font = generate_font_texture(font);
    float frequency = parser->get<float>("--frequency") * 1000000;
    int adjust = 0;

    while (!WindowShouldClose())
    {
        machine->graphics->render();
        machine->keyboard->update();

        int target_cycles = GetFrameTime() * frequency;
        int cycles_ran = machine->run(target_cycles - adjust);
        adjust = cycles_ran - target_cycles;

        SetWindowTitle(("Fantacom - " + std::to_string(GetFPS()) + " FPS").c_str());
    }

    CloseWindow();
    return 0;
}
