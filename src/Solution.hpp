#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include <memory>

#include "Function.hpp"
#include "Point.hpp"

struct Solution
{
    Solution(Function fn) : m_function(fn) { }
    Solution(Function&& fn) : m_function(std::move(fn)) {}


    void assign(int startPoint, int endPoint, int step)
    {
        for(int i = startPoint; i <= endPoint; i += step)
        {
            m_points.emplace_back(i, function()(i));
        }
    }

    const std::vector<Point>& points() const
    {
        return m_points;
    }

    Function& function() 
    {
        return m_function;
    }

private:

    Function m_function;
    std::vector<Point> m_points;
};

#endif // SOLUTION_HPP
