#include "SymbolicRegressionSolver.hpp"

#include <algorithm>

#include "Function.hpp"
#include "GeneticOperations.hpp"

// TODO: support multiple variables
constexpr const char* VARIABLE_NAME = "x";

SymbolicRegressionSolver::SymbolicRegressionSolver() :
    m_randomEngine{m_randomDevice()}
{
}

SymbolicRegressionSolver::SymbolicRegressionSolver(const Config& config, const PointList& points) :
    m_randomEngine{m_randomDevice()},
    m_config(config),
    m_points(points)
{
}

void SymbolicRegressionSolver::reset()
{
    clearSolutions();
    m_isReset = true;

    m_currentGeneration = 0;
    m_foundSolution = false;

    populate();
    checkForSolution();
}

bool SymbolicRegressionSolver::checkForSolution()
{
    if(m_solutions[0].fitnessLevel <= m_config.solutionCriterea)
    {
        m_solutions.erase(std::remove_if(m_solutions.begin(),
                    m_solutions.end(),
                    [&](const Solution& sol)
                    {
                        return sol.fitnessLevel > m_config.solutionCriterea;
                    }), m_solutions.end());
        m_foundSolution = true;
    }
    
    return foundSolution();
}

void SymbolicRegressionSolver::step()
{
    assert(m_solutions.size() != 0 && "solution size is 0");

    if(foundSolution())
        return;

    m_isReset = false;
    m_currentGeneration++;

    m_solutions = performGeneticOperations();

    sort();
    checkForSolution();
}

void SymbolicRegressionSolver::sort()
{
    std::sort(m_solutions.begin(), m_solutions.end(),
            [](const Solution& s1, const Solution& s2)
            {
                return s1.fitnessLevel < s2.fitnessLevel;
            });
}

SolutionList SymbolicRegressionSolver::solve()
{
    if(!isReset())
    {
        reset();
    }

    m_isRunning = true;

    for(int i = 0; m_isRunning && (i < m_config.maxGenerations && m_solutions.size() != 0) && !m_foundSolution; ++i)
    {
        step();
    }

    return m_foundSolution ? m_solutions : SolutionList();
}

SolutionList SymbolicRegressionSolver::performGeneticOperations()
{
    SolutionList newSolutions;
    newSolutions.reserve(m_solutions.size());

    NodePtr* matingList[2] = { nullptr, nullptr };

    RandomEngine engine(m_randomDevice());
    std::uniform_real_distribution<> dist(0, 1);

    auto endIt = m_solutions.begin() + (size_t)(m_solutions.size() * m_config.keepPercentage);

    for(auto it = m_solutions.begin(); it != endIt; ++it)
    {
        auto& solution = *it;
#ifdef VERBOSE_LOG
        std::cout << "fn: " << solution.function << '\n';
#endif
        auto randomNumber = dist(engine);
        if(m_config.initialPopulation == 1 || randomNumber <= m_config.mutationPercent)
        {
            performMutation(solution);
            newSolutions.emplace_back(solution);
        }

        if(m_config.initialPopulation == 1)
            continue;

        else if(randomNumber <= m_config.mutationPercent + m_config.matePercent)
        {
            if(matingList[0] && matingList[1])
            {
                Solution sol = createSolution(Function{mate(m_randomEngine, *matingList[0], *matingList[1], [&]()
                            {
                                NodePtr node;
                                std::uniform_int_distribution<> dist(0, 2);
                                if(dist(engine) <= 1 && m_config.generateConstantNodes)
                                {
                                    node = ::node<ValueNode>(m_config.constantDist(engine));
                                }
                                else
                                {
                                    node = ::node<VariableNode>(VARIABLE_NAME);
                                }
                                return node;
                            }, m_config.maxSolutionDepth)});
                sol.mated = true;
                newSolutions.emplace_back(sol);

#ifdef VERBOSE_LOG
                std::cout << "mating " << solution.function;
#endif 
                matingList[0] = matingList[1] = nullptr;
            }
            else
            {
                for(int i = 0; i < 2; ++i)
                {
                    if(!matingList[i]) continue;

                    matingList[i] = &solution.function.getNode();
                }
            }
        }
        else
        {
            newSolutions.emplace_back(solution);
        }
    }

    if(m_config.populationRefillOption == Config::PopulationRefillOption::DUPLICATE)
    {
        for(auto& it = endIt; it != m_solutions.end(); ++it)
        {
            newSolutions.emplace_back(*it);
        }
    }
    else if(m_config.populationRefillOption == Config::PopulationRefillOption::REFILL)
    {
        for(size_t i = 0; i < m_config.initialPopulation - newSolutions.size(); ++i)
        {
            newSolutions.emplace_back(randomlyGenerateSolution());
        }
    }
    else
    {
        // do nothing
    }

    return newSolutions;
}

void SymbolicRegressionSolver::performMutation(Solution& solution)
{
    bool useNearestNeighbour = false;
    if(m_config.nearestNeighbourOption == Config::NearestNeighbourOption::ALWAYS_USE)
    {
        useNearestNeighbour = true;
    }
    else if(m_config.nearestNeighbourOption == Config::NearestNeighbourOption::RANDOM)
    {
        useNearestNeighbour = std::uniform_real_distribution<>{0, 1}(m_randomEngine) <= m_config.chanceToUseNearestNeighbour;
    }

    mutate(m_randomEngine, solution.function.getNode(), m_config.constantDist, m_config.chanceToChangeVar, m_config.chanceToChangeConstant, useNearestNeighbour, m_config.stepSize);

    solution.fitnessLevel = fitness(solution.function);
    solution.mutated = true;

#ifdef VERBOSE_LOG
    std::cout << "mutated: " << solution.function << '\n';
#endif // VERBOSE_LOG
}


void SymbolicRegressionSolver::populate()
{
    for(int i = 0; i < m_config.initialPopulation; ++i)
    {
        m_solutions.emplace_back(randomlyGenerateSolution());
    }
    sort();
}

void SymbolicRegressionSolver::updateFitnesses()
{
    for(auto& solution : m_solutions)
    {
        solution.fitnessLevel = fitness(solution.function);
    }
}

size_t SymbolicRegressionSolver::fitness(const Function& fn)
{
    size_t result = 0;
    for(auto& p : m_points)
    {
        result += abs(p.y - fn(m_variableMap, p.x));
    }
    return result;
}

Solution SymbolicRegressionSolver::createSolution(Function fn)
{
    auto fit = fitness(fn);
    return Solution{std::move(fn), fit};
}

NodePtr SymbolicRegressionSolver::generate(int depth)
{
    static std::uniform_int_distribution<> binaryDist(0, 1);
    static std::uniform_int_distribution<> operatorDist(0, (int)Operator::COUNT - 1);
    Node::Type typeToGenerate = Node::Type::OPERATION;
    if(depth == 0)
    {
        typeToGenerate = binaryDist(m_randomEngine) == 0 ? Node::Type::VALUE : Node::Type::VARIABLE;
    }

    switch(typeToGenerate)
    {
        case Node::Type::VALUE:
            if(m_config.generateConstantNodes)
            {
                return node<ValueNode>(m_config.constantDist(m_randomEngine));
            } // else fall through
        case Node::Type::VARIABLE:
            return node<VariableNode>(VARIABLE_NAME);
        case Node::Type::OPERATION:
            {
                return node<OperatorNode>(static_cast<Operator>(operatorDist(m_randomEngine)),
                        std::move(generate(depth - 1)),
                        std::move(generate(depth - 1)));
            }
        default:
            ASSERT(true, "invalid type generated in generate");
            break;
    }

    return nullptr;
}

Solution SymbolicRegressionSolver::randomlyGenerateSolution()
{
    std::uniform_int_distribution<> dist(1, m_config.initialMaxDepth);
    const int depthToGenerate = dist(m_randomEngine);

    return createSolution(std::move(Function{generate(depthToGenerate)}));
}

std::ostream& operator<<(std::ostream& os, const SymbolicRegressionSolver::Config::NearestNeighbourOption& n)
{
    static constexpr const char* STRINGS[] = { "Always Use", "Never Use", "Random" };
    return os << STRINGS[static_cast<size_t>(n)];
}

std::ostream& operator<<(std::ostream& os, const SymbolicRegressionSolver::Config::PopulationRefillOption& p)
{
    static constexpr const char* STRINGS[] = { "Refill", "Duplicate", "Throw Away" };
    return os << STRINGS[static_cast<size_t>(p)];
}

std::ostream& operator<<(std::ostream& os, const SymbolicRegressionSolver::Config& config)
{
    os << "initial population = " << config.initialPopulation << '\n';
    os << "max generations    = " << config.maxGenerations << '\n';
    os << "initial max depth  = " << config.initialMaxDepth << '\n';
    os << "max solution depth = " << config.maxSolutionDepth << '\n';
    os << "keep percent       = " << config.keepPercentage << '\n';
    os << "mutation percent   = " << config.mutationPercent << '\n';
    os << "mate percent       = " << config.matePercent << '\n';
    os << "min constant       = " << config.constantDist.a() << '\n';
    os << "max constant       = " << config.constantDist.b() << '\n';
    os << "solution criteria  = " << config.solutionCriterea << '\n';
    os << "chance to change const = " << config.chanceToChangeConstant << '\n';
    os << "chance to change var = "  << config.chanceToChangeVar << '\n';
    os << "nearest neighbour opt = " << config.nearestNeighbourOption << '\n';
    os << "chance to use nearest neighbour = " << config.chanceToUseNearestNeighbour << '\n';
    os << "step size = " << config.stepSize << '\n';
    os << "pop refill opt = " << config.populationRefillOption << '\n';
    return os;
}
