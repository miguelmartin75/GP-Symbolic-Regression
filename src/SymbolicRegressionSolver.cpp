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
    populate();

    for(int i = 0; i < m_config.maxGenerations && m_solutions.size() != 0; ++i)
    {
        std::sort(m_solutions.begin(), m_solutions.end(),
         [](const Solution& s1, const Solution& s2) -> bool
         {
            return s1.fitnessLevel < s2.fitnessLevel;
         });
        
        std::cout << "Best solution for generation " << i + 1 << " is: ";
        std::cout << m_solutions[0].function;
        std::cout << " [fitness = " << m_solutions[0].fitnessLevel << "]\n";
        if(m_solutions[0].fitnessLevel <= m_config.solutionCriterea)
        {
            m_solutions.erase(std::remove_if(m_solutions.begin(),
                                             m_solutions.end(),
                                             [&](const Solution& sol) -> bool
                                             {
                                                return sol.fitnessLevel > m_config.solutionCriterea;
                                             }), m_solutions.end());
            return m_solutions;
        }

        m_solutions.erase(m_solutions.begin() + (int)(m_solutions.size() * m_config.keepPercentage), m_solutions.end());

        m_solutions = performGeneticOperations();
    }

    return SolutionList();
}


std::ostream& os = std::cout;

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
            mutate(m_randomEngine, solution.function, m_constantDist);
            solution.fitnessLevel = fitness(solution.function);
            solution.modified = true;
            newSolutions.emplace_back(solution);
        }
        
        else if(randomNumber <= m_config.mutationPercent + m_config.matePercent)
        {
            matingList.emplace_back(&solution.function);
            if(matingList.size() == 2)
            {
                Solution sol = createSolution(mate(m_randomEngine, *matingList[0], *matingList[1], [&]()
                {
                    NodePtr node;
                    std::uniform_int_distribution<> dist(0, 2);
                    if(dist(engine) <= 1)
                    {
                        node = ::node<ValueNode>(m_constantDist(engine));
                    }
                    else
                    {
                        node = ::node<VariableNode>("x");
                    }
                    return node;
                }, m_config.maxSolutionDepth));
                sol.modified = true;
                newSolutions.emplace_back(sol);

                matingList.clear();
            }
        }
        else
        {
            newSolutions.emplace_back(solution);
        }
    }

    for(size_t i = 0; i < m_config.initialPopulation - newSolutions.size(); ++i)
    {
        newSolutions.emplace_back(randomlyGenerateSolution());
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

size_t SymbolicRegressionSolver::fitness(Function& fn) const
{
    size_t result = 0;
    for(auto& p : m_points)
    {
        result += abs(p.y - fn(p.x));
    }
    return result;
}

Solution SymbolicRegressionSolver::createSolution(Function fn) const
{
    auto fit = fitness(fn);
    return Solution{std::move(fn), fit};
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
                                          std::move(generate(engine, depth - 1, constantDist)),
                                          std::move(generate(engine, depth - 1, constantDist)));
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
    std::uniform_int_distribution<> dist(1, m_config.initialMaxDepth);
    const int depthToGenerate = dist(m_randomEngine);
    
    return createSolution(std::move(Function{generate(m_randomEngine, depthToGenerate, m_constantDist)}));
}
