#pragma once

class QPainter;

namespace AlgoVi {
namespace Visual {

class IVisualizer
{
public:
    virtual void visualize(QPainter* painter) = 0;
};

} // namespace Visual
} // namespace AlgoVi
