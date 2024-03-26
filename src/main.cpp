#include <iostream>
#include <argparse/argparse.hpp>
#include <machine.hpp>
#include <fstream>
#include <filesystem>
#include <raylib.h>
#include <memory>
#include <unifont.hpp>
#include <cp437.hpp>

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
    auto rt = LoadRenderTexture(8 * 16, 16 * 16);
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

    auto img = LoadImageFromTexture(rt.texture);
    UnloadRenderTexture(rt);
    ImageFlipVertical(&img);
    auto tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

std::shared_ptr<Machine> setup_machine(std::shared_ptr<const argparse::ArgumentParser> args)
{
    auto machine = std::make_shared<Machine>();
    machine->ram.resize(args->get<int>("--ram") * 0x1000);
    for (int i = 0; i < machine->ram.size(); i++)
    {
        machine->ram[i] = i & 1 ? '@' : 0x19;
    }

    {
        std::ifstream bootrom(args->get<std::string>("bootrom"), std::ios::binary);

        if (!bootrom)
        {
            std::cerr << "Could not open boot ROM image" << std::endl;
            exit(1);
        }
        if (std::filesystem::file_size(args->get<std::string>("bootrom")) > machine->rom.size())
        {
            std::cerr << "WARN: Boot ROM image too large, truncating" << std::endl;
        }

        bootrom.read((char *)machine->rom.data(), machine->rom.size());
    }

    for (auto filename : args->get<std::vector<std::string>>("disk"))
    {
        auto stream = std::make_shared<std::ifstream>(filename, std::ios::binary);
        if (!*stream)
        {
            std::cout << "Unable to open disk image `" << filename << "`" << std::endl;
            exit(1);
        }
        machine->disk_ctrl->disks.push_back(Disk(stream));
    }

    return machine;
}

void present_rt(RenderTexture2D rt)
{
    auto texture = rt.texture;
    DrawTexturePro(texture, {0, 0, (float)texture.width, (float)-texture.height}, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
}

int main(int argc, char *argv[])
{
    auto args = parse_args(argc, argv);
    auto machine = setup_machine(args);

    std::ifstream font_file("font.hex");
    if (!font_file)
        throw std::runtime_error("Failed to open font file");
    Unifont::Font font(font_file);

    InitWindow(1280, 800, "Fantacom - Initializing...");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    machine->graphics->init();
    machine->graphics->font = generate_font_texture(font);
    float frequency = args->get<float>("--frequency") * 1000000;
    int adjust = 0;

    while (!WindowShouldClose())
    {
        int target_cycles = GetFrameTime() * frequency;
        int cycles_ran = machine->run(target_cycles - adjust);
        adjust = cycles_ran - target_cycles;

        machine->graphics->render();
        machine->keyboard->update();

        SetWindowTitle(("Fantacom - " + std::to_string(GetFPS()) + " FPS").c_str());
        BeginDrawing();
        present_rt(machine->graphics->screen_rt);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
