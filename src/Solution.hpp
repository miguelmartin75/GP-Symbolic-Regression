#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include <memory>

#include "Function.hpp"
#include "Point.hpp"

struct Solution
{
    // TODO: remove -1 
    Solution(Function function, int fitnessLevel = -1) :
        function(function),
        fitnessLevel(fitnessLevel)
    {
    }

    Function function;
    int fitnessLevel;
};

#endif // SOLUTION_HPP
