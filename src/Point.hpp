#ifndef POINT_HPP
#define POINT_HPP

struct Point
{
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    int x;
    int y;
};

bool operator<(const Point& p1, const Point& p2)
{
    return p1.x < p2.x && p2.y < p2.y;
}

bool operator>=(const Point& p1, const Point& p2)
{
    return !operator<(p1, p2);
}

bool operator<=(const Point& p1, const Point& p2)
{
    return !operator<(p2, p1);
}

bool operator>(const Point& p1, const Point& p2)
{
    return !operator<=(p1, p2);
}

bool operator==(const Point& p1, const Point& p2)
{
    return p1.x == p2.x && p2.y == p2.y;
}

bool operator!=(const Point& p1, const Point& p2)
{
    return !operator==(p1, p2);
}

#endif // POINT_HPP
