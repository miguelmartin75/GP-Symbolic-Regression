#include "SymbolicRegressionSolver.hpp"

#include <algorithm>

#include "Function.hpp"
#include "GeneticOperations.hpp"

SymbolicRegressionSolver::SymbolicRegressionSolver(const Config& config, const PointList& points) :
    m_config(config),
    m_points(points),
    m_randomEngine(m_randomDevice())
{
    if(config.useMaxMinRandom)
    {
        m_randomDist = decltype(m_randomDist)(config.minimumRandom, config.maximumRandom);
    }
}

Solution SymbolicRegressionSolver::solve()
{
    populate();

    /*
    std::sort(m_solutions.begin(), m_solutions.end(), 
     [](const Solution& s1, const Solution& s2)
     {
     });
     */

    // pick the best 10% or w/e
    //m_solutions.erase(std::remove(m_solutions.begin(), m_solutions.end() + (m_solutions.size() * 0.10), m_solutions.end()));

    performGeneticOperations();
    return Solution{Function{"(+ 3 2)"}};
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

// TODO: do I really need this? Lol
void SymbolicRegressionSolver::computeFitness()
{
    for(auto& solution : m_solutions)
    {
        solution.fitnessLevel = fitness(solution.function);
    }
}

int SymbolicRegressionSolver::fitness(Function& fn) const
{
    int result = m_points.size();
    for(auto& p : m_points)
    {
        if(p.y == fn(p.x))
        {
            --result; 
        }
    }
    return result;
}

Solution SymbolicRegressionSolver::randomlyGenerateSolution()
{
    return Solution{Function{"TODO"}};
}

Solution SymbolicRegressionSolver::createSolution(Function& fn) const
{
    return Solution{fn, fitness(fn)};
}
