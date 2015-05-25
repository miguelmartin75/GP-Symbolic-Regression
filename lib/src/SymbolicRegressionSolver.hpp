#ifndef SYMBOLICREGRESSIONSOLVER_HPP
#define SYMBOLICREGRESSIONSOLVER_HPP

#include <vector>
#include "Config.hpp"
#include "Types.hpp"

#include "random_utils.hpp"

class SymbolicRegressionSolver
{
public:

    struct Config
    {
        int initialPopulation = 1000;

        int maxGenerations = 100;
        int initialMaxDepth = 5;
        int maxSolutionDepth = 20;

        // TODO: random removes from various positions
        float keepPercentage = 0.10f;

        // if these two numbers do not add up to 1.0
        // then it is assumed that we will duplicate for the rest of it
        float mutationPercent = 0.45f;
        float matePercent = 0.45f;

        // max/min random co-efficients
        std::uniform_int_distribution<> constantDist{0, 100};
        
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

        enum class PopulationRefillOption
        {
            REFILL,
            DUPLICATE,
            THROW_AWAY
        } populationRefillOption = PopulationRefillOption::REFILL;
    };

    SymbolicRegressionSolver();
    SymbolicRegressionSolver(const Config& config, const PointList& points);

    void performMutation(Solution& s);
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

    void setIsRunning(bool b) { m_isRunning = b; }
    bool isRunning() const { return m_isRunning; }
    void stop() { m_isRunning = false; }
    bool isReset() const { return m_isReset; }

    Config& config() { return m_config; }
    const Config& config() const { return m_config; }

    bool foundSolution() { return m_foundSolution; }

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
    int m_currentGeneration = 0;
    bool m_foundSolution = false;
    bool m_isRunning = false;
    bool m_isReset = false;
};

#endif // SYMBOLICREGRESSIONSOLVER_HPP
