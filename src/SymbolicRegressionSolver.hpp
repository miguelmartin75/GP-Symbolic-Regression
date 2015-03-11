#ifndef SYMBOLICREGRESSIONSOLVER_HPP
#define SYMBOLICREGRESSIONSOLVER_HPP

#include <vector>
#include <random>

#include "Point.hpp"
#include "Solution.hpp"

using PointList = std::vector<Point>;

class SymbolicRegressionSolver
{
public:

    struct Config
    {
        int initialPopulation = 500;
        int maxGenerations = 100;
        int initialMaxDepth = 5;
        int maxSolutionDepth = 50;
        float crossoverPercent = 0.45f;
        float mutationPercent = 0.45f;
        float duplicationPercent = 1.0f - crossoverPercent + mutationPercent;

        // max/min random co-efficients
        bool useMaxMinRandom = true;
        int minimumRandom = -50;
        int maximumRandom = 50;
        
        int solutionCriterea = 0; /* 0 means fully successful */
    };

    SymbolicRegressionSolver(const Config& config, const PointList& points);

    Solution solve();

private:

    void populate();
    void performGeneticOperations();
    Solution randomlyGenerateSolution();
    void computeFitness();
    int fitness(Function& fn) const;
    Solution createSolution(Function& fn) const; 
    
    Config m_config;
    PointList m_points;
    std::vector<Solution> m_solutions;

    std::random_device m_randomDevice;
    std::default_random_engine m_randomEngine;
    std::uniform_int_distribution<int> m_randomDist;
};

#endif // SYMBOLICREGRESSIONSOLVER_HPP
