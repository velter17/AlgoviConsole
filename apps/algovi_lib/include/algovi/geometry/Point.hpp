#pragma once

#include <cstdint>
#include <cmath>
#include <tuple>

namespace AlgoVi {

template <typename T>
class Point_
{
public:
    Point_(const T& x, const T& y)
        : m_x(x)
        , m_y(y)
    {
    }

    Point_()
        : Point_(0, 0)
    {
    }

    T x() const
    {
        return m_x;
    }

    T y() const
    {
        return m_y;
    }

    void setX(const T& x) 
    {
        m_x = x;
    }

    void setY(const T& y)
    {
        m_y = y;
    }

    Point_ operator+(const Point_& other) const
    {
        return Point_(x() + other.x(), y() + other.y());
    }

    Point_ operator-(const Point_& other) const
    {
        return Point_(x() - other.x(), y() - other.y());
    }

private:
    T m_x;
    T m_y;
};

using Point = Point_<double>;
using PointF = Point_<float>;
using PointI = Point_<int32_t>;
using PointL = Point_<int64_t>;

template <typename T>
class Vector_ : public Point_<T>
{
public:
    Vector_(const Point_<T>& from, const Point_<T>& to)
        : Point_<T>(to - from)
    {
    }

    Vector_()
    {
    }

    T length2() const
    {
        return this->x() * this->x() + this->y() * this->y();
    }
};

template <typename T>
inline double length(const Vector_<T>& v)
{
    return std::sqrt(v.length2());
}

using Vector = Vector_<double>;
using VectorF = Vector_<float>;
using VectorI = Vector_<int32_t>;
using VectorL = Vector_<int64_t>;

template <typename T>
bool operator<(const Point_<T>& lhs, const Point_<T>& rhs)
{
    return std::tie(lhs.x(), lhs.y()) < std::tie(rhs.x(), rhs.y());
}

template <typename T>
bool operator<(const Vector_<T>& lhs, const Vector_<T>& rhs)
{
    return std::tie(lhs.x(), lhs.y()) < std::tie(rhs.x(), rhs.y());
}

} // namespace AlgoVi
