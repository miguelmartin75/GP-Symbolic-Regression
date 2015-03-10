#include "SymbolicRegressionSolver.hpp"

#include <algorithm>

SymbolicRegressionSolver::SymbolicRegressionSolver(const Config& config, const PointList& points) :
    m_config(config),
    m_points(points)
{
}

Solution SymbolicRegressionSolver::solve()
{
    populate();     

    std::sort(m_solutions.begin(), m_solutions.end());

    // pick the best 10% or w/e
    m_solutions.erase(std::remove(m_solutions.begin(), m_solutions.end() + (m_solutions.size() * 0.10), m_solutions.end()));

    performGeneticOperations();
}

void SymbolicRegressionSolver::populate()
{
    for(int i = 0; i < m_config.initialPopulation; ++i)
    {
        m_solutions.emplace_back(randomlyGenerateSolution());
    }
}

void SymbolicRegressionSolver::performGeneticOperations()
{
    // TODO
}
