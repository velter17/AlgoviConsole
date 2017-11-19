#pragma once

#include <cassert>
#include <cstdint>

#include "algovi/paint/Color.hpp"

namespace AlgoVi {

class Pen
{
public:
    Pen(uint32_t width = 1, const Color_& color = makeColor<EColor::Black>())
        : m_width(width)
        , m_color(color)
    {
        assert(width < 100 && "are you sure about such huge width of pen?");
    }

    uint32_t width() const { return m_width; }
    Color_ color() const { return m_color; }

private:
    const Color_ m_color;
    uint32_t m_width;
};

} // namespace AlgoVi
