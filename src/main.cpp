#include <iostream>
#include <string>

#include "Point.hpp"
#include "Function.hpp"
#include "SymbolicRegressionSolver.hpp"

Function fn{"(+ 1 (* x x))"};
SymbolicRegressionSolver::Config config{};
int steps;

int main(int argc, char *argv[])
{
    std::cout << "steps: ";
    std::cin >> steps;
    std::cin.ignore();

    std::vector<Point> points;
    points.reserve(steps);

    const int halfOfSteps = (steps/2);
    const int startPoint = -halfOfSteps;
    const int endPoint = halfOfSteps;

    for(int i = startPoint; i <= endPoint; ++i)
    {
        points.emplace_back(i, fn(i));
    }

    std::cout << "\npoints for function " << fn << ":\n";

    for(auto& p : points) 
    {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }

    SymbolicRegressionSolver solver(config, points);
    Solution solution = solver.solve();

    std::cout << "function: " << solution.function << '\n';
    std::cout << "fitness: " << solution.fitnessLevel << '\n';
    
    return 0;
}
