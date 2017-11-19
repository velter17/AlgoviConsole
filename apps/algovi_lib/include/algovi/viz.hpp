#pragma once

#include <iostream>
#include <stack>
#include "algovi/geometry/Point.hpp"
#include "algovi/paint/Color.hpp"
#include "algovi/paint/Pen.hpp"
#include "algovi/paint/Brush.hpp"

namespace AlgoVi {

template <typename T>
void drawCircle(const Point_<T>& p, double radius)
{
    std::cout << "circle " << p.x() << " " << p.y() << " " << radius << std::endl;
}

template <typename T>
void drawLine(const Point_<T>& from, const Point_<T>& to)
{
    std::cout << "line " << from.x() << " " << from.y() << " " << to.x() << " " << to.y()
              << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const Color_& color)
{
    return stream << color.R << " " << color.G << " " << color.B;
}

class Painter
{
public:
    static Painter& getInstance()
    {
        static Painter instance;
        return instance;
    }

    Pen pen() const
    {
        return m_stack.top().first;
    }

    Brush brush() const
    {
        return m_stack.top().second;
    }

    void set(const Pen& pen)
    {
        Brush b = brush();
        m_stack.pop();
        push(pen, b);
    }

    void set(const Brush& brush)
    {
        Pen p = pen();
        m_stack.pop();
        push(p, brush);
    }

    void push(const Pen& pen, const Brush& brush)
    {
        m_stack.emplace(pen, brush);
        update();
    }

    bool pop()
    {
        if (m_stack.size() == 1)
        {
            return false;
        }
        else
        {
            m_stack.pop();
            update();
            return true;
        }
    }
private:
    Painter()
    {
        push(Pen(), Brush());
    }

    void update()
    {
        std::cout << "pen width " << m_stack.top().first.width() << "\n";
        std::cout << "pen color " << m_stack.top().first.color() << "\n";
        std::cout << "brush color " << m_stack.top().second.color() << std::endl;
    }

private:
    std::stack<std::pair<Pen, Brush>> m_stack;
};

Painter& painter()
{
    return Painter::getInstance();
}

} // namespace AlgoVi
