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
        int initialPopulation = 1600;
        int maxGenerations = 300;
        int initialMaxDepth = 5;
        int maxSolutionDepth = 20;

        // TODO: random removes from various positions
        float keepPercentage = 0.90f;

        // if these three numbers do not add up to 1.0
        // then it is assumed that we will duplicate for the rest of it
        float mutationPercent = 0.45f;
        float matePercent = 0.45f;
        

        // max/min random co-efficients
        bool useMaxMinRandom = true;
        int minimumConstantValue = 0;
        int maximumConstantValue = 1000;
        
        size_t solutionCriterea = 0; /* 0 means fully successful */
    };

    SymbolicRegressionSolver(const Config& config, const PointList& points);

    void step();
    int currentGeneration() const { return m_currentGeneration; }
    void reset();
    const SolutionList& currentSolutionSet() const { return m_solutions; }

    SolutionList solve();

public:

    void populate();
    SolutionList performGeneticOperations();
    Solution randomlyGenerateSolution();
    void updateFitnesses();
    size_t fitness(Function& fn) const;
    Solution createSolution(Function fn) const;
    
    Config m_config;
    PointList m_points;
    SolutionList m_solutions;

    std::random_device m_randomDevice;
    RandomEngine m_randomEngine;
    std::uniform_int_distribution<int> m_constantDist;
    int m_currentGeneration = 0;
    bool m_foundSolution = false;
};

#endif // SYMBOLICREGRESSIONSOLVER_HPP