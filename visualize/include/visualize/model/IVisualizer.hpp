#pragma once

class QPainter;

namespace SOME_NAME {
namespace Visual {

class IVisualizer
{
public:
    virtual void visualize(QPainter* painter) = 0;
};

} // namespace Visual
} // namespace SOME_NAME
