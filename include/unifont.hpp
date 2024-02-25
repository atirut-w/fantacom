#pragma once
#include <cstdint>
#include <map>
#include <istream>

namespace Unifont
{
    class Glyph
    {
    public:
        int width;
        uint16_t data[16];

        bool operator >(const Glyph &other) const
        {
            return width > other.width;
        }
    };

    class Font
    {
    public:
        std::map<int, Glyph> glyphs;

        Font(std::istream &stream);
    };
}
