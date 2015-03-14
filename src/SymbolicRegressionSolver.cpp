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
        m_constantDist = decltype(m_constantDist)(config.minimumConstantValue, config.maximumConstantValue);
    }
}

Solution SymbolicRegressionSolver::solve()
{
    populate();

    std::sort(m_solutions.begin(), m_solutions.end(), 
     [](const Solution& s1, const Solution& s2) -> bool
     {
        return s1.fitnessLevel < s2.fitnessLevel;
     });

    m_solutions.erase(m_solutions.begin() + (int)(m_solutions.size() * (1.0 - m_config.removePercent)), m_solutions.end());

    m_solutions = performGeneticOperations();
    return Solution{Function{"(+ 3 2)"}}; // TODO
}

SymbolicRegressionSolver::SolutionList SymbolicRegressionSolver::performGeneticOperations()
{
    SolutionList newSolutions;
    newSolutions.reserve(m_solutions.size());

    std::vector<Function*> matingList;
    RandomEngine engine(m_randomDevice());
    std::uniform_real_distribution<> dist(0, 1);

    for(auto& solution : m_solutions)
    {
        auto randomNumber = dist(engine);
        if(randomNumber <= m_config.mutationPercent)
        {
            newSolutions.emplace_back(createSolution(mutate(m_randomEngine, solution.function, m_constantDist)));
        }
        else if(randomNumber <= m_config.mutationPercent + m_config.matePercent)
        {
            if(matingList.size() == 2)
            {
                newSolutions.emplace_back(createSolution(mate(m_randomEngine, *matingList[0], *matingList[1])));
                matingList.clear();
            }

            matingList.emplace_back(&solution.function);
        }
        // duplicate
        else
        {
            newSolutions.emplace_back(solution);
        }
    }

    return newSolutions;
}

void SymbolicRegressionSolver::populate()
{
    for(int i = 0; i < m_config.initialPopulation; ++i)
    {
        m_solutions.emplace_back(randomlyGenerateSolution());
    }
}

void SymbolicRegressionSolver::updateFitnesses()
{
    for(auto& solution : m_solutions)
    {
        solution.fitnessLevel = fitness(solution.function);
    }
}

int SymbolicRegressionSolver::fitness(Function fn) const
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

Solution SymbolicRegressionSolver::createSolution(Function fn) const
{
    return Solution{fn, fitness(fn)};
}

Solution SymbolicRegressionSolver::randomlyGenerateSolution()
{
    return Solution{Function{"TODO"}};
}
