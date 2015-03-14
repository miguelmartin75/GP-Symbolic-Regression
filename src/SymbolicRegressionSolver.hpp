#ifndef SYMBOLICREGRESSIONSOLVER_HPP
#define SYMBOLICREGRESSIONSOLVER_HPP

#include <vector>

#include "Point.hpp"
#include "Solution.hpp"

#include "random_utils.hpp"

using PointList = std::vector<Point>;
using SolutionList = std::vector<Solution>;

class SymbolicRegressionSolver
{
public:

    struct Config
    {
        int initialPopulation = 1000;
        int maxGenerations = 200;
        int initialMaxDepth = 10;
        int maxSolutionDepth = 50;

        // TODO: random removes ? in the top %x? 
        float removePercent = 0.4f;

        // if these two numbers do not add up to 1.0
        // then it is assumed that we will duplicate for the rest of it
        float mutationPercent = 0.7f;
        //float matePercent = 0.45f;
        

        // max/min random co-efficients
        bool useMaxMinRandom = true;
        int minimumConstantValue = -50;
        int maximumConstantValue = 50;
        
        int solutionCriterea = 0; /* 0 means fully successful */
    };

    SymbolicRegressionSolver(const Config& config, const PointList& points);

    SolutionList solve();

private:

    void populate();
    SolutionList performGeneticOperations();
    Solution randomlyGenerateSolution();
    void updateFitnesses();
    int fitness(Function fn) const;
    Solution createSolution(Function fn) const; 
    
    Config m_config;
    PointList m_points;
    SolutionList m_solutions;

    std::random_device m_randomDevice;
    RandomEngine m_randomEngine;
    std::uniform_int_distribution<int> m_constantDist;
};

#endif // SYMBOLICREGRESSIONSOLVER_HPP
