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

SolutionList SymbolicRegressionSolver::solve()
{
    std::cout << "populating shit\n";
    populate();

    for(int i = 0; i < m_config.maxGenerations && m_solutions.size() != 0; ++i)
    {
        std::sort(m_solutions.begin(), m_solutions.end(),
         [](const Solution& s1, const Solution& s2) -> bool
         {
            return s1.fitnessLevel < s2.fitnessLevel;
         });
        
        std::cout << "Best solution for generation " << i << " is: ";
        std::cout << m_solutions[0].function;
        std::cout << " [fitness = " << m_solutions[0].fitnessLevel << "]\n";
        if(m_solutions[0].fitnessLevel == m_config.solutionCriterea)
        {
            m_solutions.erase(std::remove_if(m_solutions.begin(),
                                             m_solutions.end(),
                                             [&](const Solution& sol) -> bool
                                             {
                                                return sol.fitnessLevel != m_config.solutionCriterea;
                                             }), m_solutions.end());
            break;
        }

        m_solutions.erase(m_solutions.begin() + (int)(m_solutions.size() * (1.0 - m_config.removePercent)), m_solutions.end());

        m_solutions = performGeneticOperations();
    }

    return m_solutions;
}

SolutionList SymbolicRegressionSolver::performGeneticOperations()
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
        /*
        else if(randomNumber <= m_config.mutationPercent + m_config.matePercent)
        {
            if(matingList.size() == 2)
            {
                newSolutions.emplace_back(createSolution(mate(m_randomEngine, *matingList[0], *matingList[1])));
                matingList.clear();
            }

            matingList.emplace_back(&solution.function);
        }
        */
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
    int result = (int)m_points.size();
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

NodePtr generate(RandomEngine& engine, int depth, std::uniform_int_distribution<>& constantDist)
{
    static std::uniform_int_distribution<> binaryDist(0, 1);
    static std::uniform_int_distribution<> operatorDist(0, (int)Operator::COUNT - 1);
    Node::Type typeToGenerate = Node::Type::OPERATION;
    if(depth == 0)
    {
        typeToGenerate = binaryDist(engine) == 0 ? Node::Type::VALUE : Node::Type::VARIABLE;
    }

    switch(typeToGenerate)
    {
        case Node::Type::VARIABLE:
            return node<VariableNode>("x"); 
        case Node::Type::VALUE:
            return node<ValueNode>(constantDist(engine));
        case Node::Type::OPERATION:
            {
                return node<OperatorNode>(static_cast<Operator>(operatorDist(engine)),
                                           generate(engine, depth - 1, constantDist),
                                           generate(engine, depth - 1, constantDist));
            }
        default:
            std::cerr << "Invalid type generated\n";
            std::exit(1);
            return nullptr;
            break;
    }
}

Solution SymbolicRegressionSolver::randomlyGenerateSolution()
{
    std::uniform_int_distribution<> dist(0, m_config.initialMaxDepth);
    const int depthToGenerate = dist(m_randomEngine);
    
    return createSolution(Function{generate(m_randomEngine, depthToGenerate, m_constantDist)});
}
