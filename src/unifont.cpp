#include <unifont.hpp>
#include <string>
#include <stdexcept>

using namespace Unifont;

Font::Font(std::istream &stream)
{
    std::string line;
    while (std::getline(stream, line))
    {
        int code = std::stoi(line.substr(0, line.find(':')), nullptr, 16);
        line = line.substr(line.find(':') + 1);

        Glyph glyph;
        glyph.width = line.length() / 4;
        if (glyph.width != 8 && glyph.width != 16)
        {
            throw std::runtime_error("Invalid glyph width");
        }

        for (int i = 0; i < 16; i++)
        {
            if (glyph.width == 8)
                glyph.data[i] = std::stoi(line.substr(i * 2, 2), nullptr, 16);
            else
                glyph.data[i] = std::stoi(line.substr(i * 4, 4), nullptr, 16);
        }

        glyphs[code] = glyph;
    }
}
