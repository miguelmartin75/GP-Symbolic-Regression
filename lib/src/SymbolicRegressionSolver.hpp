#ifndef SYMBOLICREGRESSIONSOLVER_HPP
#define SYMBOLICREGRESSIONSOLVER_HPP

#include <vector>

#include "Point.hpp"
#include "Solution.hpp"
#include "Config.hpp"

#include "random_utils.hpp"

using PointList = std::vector<Point>;
using SolutionList = std::vector<Solution>;

class SymbolicRegressionSolver
{
public:

    struct Config
    {
#ifdef ONE_POPULATION
        int initialPopulation = 1;
#else
        int initialPopulation = 1600;
#endif
        int maxGenerations = 300;
        int initialMaxDepth = 5;
        int maxSolutionDepth = 20;

        // TODO: random removes from various positions
        float keepPercentage = 0.10f;

        // if these three numbers do not add up to 1.0
        // then it is assumed that we will duplicate for the rest of it
        float mutationPercent = 0.45f;
        float matePercent = 0.45f;
        

        // max/min random co-efficients
        bool useMaxMinRandom = true;
        int minimumConstantValue = 0;
        int maximumConstantValue = 100;
        
        size_t solutionCriterea = 0; /* 0 means fully successful */

        double chanceToChangeConstant = 0.3;
        double chanceToChangeVar = 0.4;
         
        enum class NearestNeighbourOption
        {
            ALWAYS_USE,
            NEVER_USE,
            RANDOM
        } nearestNeighbourOption = NearestNeighbourOption::RANDOM;
        double chanceToUseNearestNeighbour = 0.7;
        int stepSize = 1; /* step for nearest neightbour */
    };

    SymbolicRegressionSolver();
    SymbolicRegressionSolver(const Config& config, const PointList& points);

    void step();
    int currentGeneration() const { return m_currentGeneration; }
    void reset();
    void clearSolutions() { m_solutions.clear(); }
    const SolutionList& currentSolutionSet() const { return m_solutions; }
    Function& getFunction(int index) { return m_solutions[index].function; }

    SolutionList solve();

    void setPoints(PointList points) { m_points = std::move(points); }
    PointList& points() { return m_points; }
    const PointList& points() const { return m_points; }

    bool isRunning() const { return m_isRunning; }
    void stop() { m_isRunning = false; }
    bool isReset() const { return m_isReset; }

private:

    void sort();
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
    bool m_isRunning = false;
    bool m_isReset = false;
};

#endif // SYMBOLICREGRESSIONSOLVER_HPP
