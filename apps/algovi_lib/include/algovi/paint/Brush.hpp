#pragma once

#include "algovi/paint/Color.hpp"

namespace AlgoVi {

class Brush
{
public:
    Brush(const Color_& color = makeColor<EColor::Black>())
        : m_color(color)
    {
    }

    Color_ color() const { return m_color; }

private:
    const Color_ m_color;
};

} // namespace AlgoVi
