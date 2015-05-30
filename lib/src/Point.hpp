#ifndef POINT_HPP
#define POINT_HPP

#include "Config.hpp"

struct Point
{
    Point() : x(0), y(0) {}
    Point(Value x, Value y) : x(x), y(y) {}

    Value x;
    Value y;
};

inline bool operator<(const Point& p1, const Point& p2)
{
    return p1.x < p2.x;
}

inline bool operator==(const Point& p1, const Point& p2)
{
    return p1.x == p2.x && p2.y == p2.y;
}

inline bool operator!=(const Point& p1, const Point& p2)
{
    return !operator==(p1, p2);
}

#endif // POINT_HPP
