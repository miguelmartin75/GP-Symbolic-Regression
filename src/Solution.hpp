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
        function(function),
        fitnessLevel(fitnessLevel)
    {
    }

    Function function;
    size_t fitnessLevel;
    bool newSolution = true;
};

#endif // SOLUTION_HPP
