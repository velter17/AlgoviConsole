#pragma once

#include <iostream>
#include "algovi/geometry/Point.hpp"

namespace AlgoVi {

template <typename T>
bool readPoint(Point_<T>& p)
{
    double x, y;
    if (std::cin >> x >> y)
    {
        p.setX(x);
        p.setY(y);
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace AlgoVi
