#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include <memory>

#include "Function.hpp"
#include "Point.hpp"

struct Solution
{
    // TODO: remove -1 
    Solution(Function function, size_t fitnessLevel) :
        function(std::move(function)),
        fitnessLevel(fitnessLevel)
    {
    }

    Function function;
    size_t fitnessLevel;
    bool modified = false;
};

#endif // SOLUTION_HPP
