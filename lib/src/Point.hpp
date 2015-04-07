#ifndef POINT_HPP
#define POINT_HPP

struct Point
{
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    int x;
    int y;
};

inline bool operator<(const Point& p1, const Point& p2)
{
    return p1.x < p2.x && p2.y < p2.y;
}

inline bool operator>=(const Point& p1, const Point& p2)
{
    return !operator<(p1, p2);
}

inline bool operator<=(const Point& p1, const Point& p2)
{
    return !operator<(p2, p1);
}

inline bool operator>(const Point& p1, const Point& p2)
{
    return !operator<=(p1, p2);
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
