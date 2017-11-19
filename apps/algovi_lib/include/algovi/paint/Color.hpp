#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <ostream>

namespace AlgoVi {

struct Color_
{
    Color_(int32_t r, int32_t g, int32_t b)
        : R(r)
        , G(g)
        , B(b)
    {
    }

    const int32_t R;
    const int32_t G;
    const int32_t B;
};

using Color = Color_;

template <int r, int g, int b>
struct TColor : public Color_
{
    TColor()
        : Color_(r, g, b)
    {}
};

enum class EColor
{
    Black,
    White,
    Red,
    Green,
    Blue,
};

template<EColor color>
struct makeColor
{
};

template <>
struct makeColor<EColor::Black> : public TColor<0, 0, 0>
{
};

template <>
struct makeColor<EColor::White> : public TColor<255, 255, 255>
{
};

template<>
struct makeColor<EColor::Red> : public TColor<255, 0, 0>
{
};

template <>
struct makeColor<EColor::Green> : public TColor<0, 255, 0>
{
};

template <>
struct makeColor<EColor::Blue> : public TColor<0, 0, 255>
{
};

} // namespace AlgoVi
