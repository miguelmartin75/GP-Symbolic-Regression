#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <thread>
#include <cmath>

#include <boost/lexical_cast.hpp>

#include "Point.hpp"
#include "Function.hpp"
#include "SymbolicRegressionSolver.hpp"
#include "Config.hpp"

#define OPTIMISE_CONFIG

int amountOfSimulationsToPerform = 1;
SymbolicRegressionSolver::Config config{};
Function fn{parse("+ 1 x").statement, "x"};
int initialPoint = -10;
int endPoint = 10;
int stepSize = 1;

std::istream& operator>>(std::istream& is, SymbolicRegressionSolver::Config& config);
std::ostream& operator<<(std::ostream& os, const SymbolicRegressionSolver::Config& config);

void parseArguments(int argc, char *argv[]);
void printValidFlags()
{
    std::cout << "-c <configuration-file> (TODO)\n";
    std::cout << "-f <function>\n";
    std::cout << "-i <initial-point>\n";
    std::cout << "-e <end-point>\n";
    std::cout << "-s <step size>\n";
    std::cout << "-r <amount of times to redo simulation>\n";
}

//#define MUTATE_MATE

struct Result
{
    Result(double var, double average, size_t totalSolutions, double stdDev) : var(var), average(average), totalSolutions(totalSolutions), stdDev(stdDev) { }
    Result(double var, double var2, double average, size_t totalSolutions, double stdDev) : var(var), var2(var2), average(average), totalSolutions(totalSolutions), stdDev(stdDev) { }
    double var;
    double var2;
    double average;
    size_t totalSolutions;
    double stdDev;
};

constexpr const int AMOUNT_OF_THREADS = 4;
std::vector<Result> solutions[AMOUNT_OF_THREADS];


template <class T>
void optimiseConfig(size_t id, const PointList& points, const T& start, const T& end, const T& step)
{
    SymbolicRegressionSolver solver{config, points};
#ifndef MUTATE_MATE
    auto& variableToModify = solver.config().keepPercentage;
#else
    auto& variableToModify = solver.config().matePercent;
#endif

    std::vector<size_t> temp;

#ifdef OPTIMISE_CONFIG
#ifdef MUTATE_MATE
    auto& var2 = solver.config().mutationPercent;
    for(var2 = start; var2 <= end; var2 += step)
#endif
#ifndef MUTATE_MATE
    for(variableToModify = start; variableToModify <= end; variableToModify += step)
#else
    for(variableToModify = 0; variableToModify <= 1; variableToModify += step)
#endif
    {
        double currentAverage = 0;
        size_t totalSolutions = 0;
#endif // OPTIMISE_CONFIG

        for(int i = 0; i < amountOfSimulationsToPerform; ++i)
        {
            auto solutions = solver.solve();

            for(auto& solution : solutions)
            {
#ifndef OPTIMISE_CONFIG
                std::cout << solver.currentGeneration() << ",";
                std::cout << solution.fitnessLevel << ",";
                std::cout << solution.mutated << ","; 
                std::cout << solution.mated << '\n';
#endif
            }
#ifdef OPTIMISE_CONFIG
            totalSolutions += solutions.size();
            currentAverage += solver.currentGeneration();
            temp.emplace_back(solver.currentGeneration());
#endif // OPTIMISE_CONFIG
            /*

#ifndef MUTATE_MATE
            ::solutions[id].emplace_back(variableToModify, solver.currentGeneration(), solutions.size());
#else
            ::solutions[id].emplace_back(var2, variableToModify, solver.currentGeneration(), solutions.size());
#endif 
*/

            solver.reset();
        }
        
#ifdef OPTIMISE_CONFIG
        currentAverage /= amountOfSimulationsToPerform;

        double stdDev = 0;
        for(auto i : temp)
        {
            auto diff = i - currentAverage;
            stdDev += diff * diff;
        }
        stdDev /= amountOfSimulationsToPerform;

#ifndef MUTATE_MATE
        solutions[id].emplace_back(variableToModify, currentAverage, totalSolutions, stdDev);
#else
        solutions[id].emplace_back(var2, variableToModify, currentAverage, totalSolutions, stdDev);
#endif 
    }
#endif // OPTIMISE_CONFIG
}


int main(int argc, char *argv[])
{
    parseArguments(argc, argv);

#ifdef VERBOSE_LOG
    std::clog << "function: " << fn << '\n';
    std::clog << "initial point: " << initialPoint << '\n';
    std::clog << "end point: " << endPoint << '\n';
    std::clog << "step size: " << stepSize << '\n';
#endif // VERBOSE_LOG

    std::vector<Point> points;
    points.reserve((endPoint - initialPoint)/stepSize);

    VariableMap map;
    for(int i = initialPoint; i <= endPoint; i += stepSize)
    {
        points.emplace_back(i, fn(map, i));
    }

    std::vector<std::thread> threads;
    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        constexpr double STEP_SIZE = 0.005;
        double start = i * 1.0 / AMOUNT_OF_THREADS;
        double end = (i + 1) * 1.0 / AMOUNT_OF_THREADS;
  
        threads.emplace_back([=]() { optimiseConfig(i, points, start, end,  STEP_SIZE); } );
    }

    for(auto& t : threads)
    {
        t.join();
    }

    std::vector<Result> sols;
    for(auto& sol : solutions)
    {
        //std::cout << "size of sol is: " << sol.size() << '\n';
        sols.insert(sols.end(), sol.begin(), sol.end());
    }
    std::sort(sols.begin(), sols.end(), [](const Result& r1, const Result& r2) { return r1.var < r2.var; });

    for(auto& sol : sols)
    {
#ifndef MUTATE_MATE
        std::cout << sol.var << ", " << sol.average << ", " << sol.totalSolutions << ", " << sol.stdDev << '\n';
#else
        std::cout << sol.var << ", " << sol.var2 << ", " << sol.average << ", " << sol.totalSolutions << ", ", sol.stdDev << '\n';
#endif // MUTATE_MATE
    }

    return 0;
}


void parseArguments(int argc, char *argv[])
{
    for(int i = 1; i < argc; ++i)
    {
        auto command = argv[i];
        auto commandSize = strlen(command);
        if(command[0] != '-')
        {
            std::cout << "Invalid format: " << command << ".\n";
            std::cout << "Flags must be prefixed with \"-\" (without quotes).\n";
            std::exit(-2);
            break;
        }

        if(commandSize == 1 || commandSize > 2)
        {
            std::cout << "Invalid flag: \"" << command << "\"\n";
            printValidFlags();
            std::exit(-5);
        }

        if(i + 1 >= argc)
        {
            std::cout << "please provide info with a flag...\n";
            printValidFlags();
            std::exit(-6);
        }

        switch(command[1])
        {
            // assign function
            case 'f':
                {
                    std::string functionAsString;
                    for(int j = i + 1; j < argc && argv[j][0] != '-'; ++j, ++i)
                    {
                        functionAsString += argv[j];
                    }
                    fn = parse(functionAsString).statement;
                }
                break;
            case 'i':
                {
                    std::string str = argv[++i];
                    initialPoint = boost::lexical_cast<int>(str);
                }
                break;
            case 'e':
                {
                    std::string str = argv[++i];
                    endPoint = boost::lexical_cast<int>(str);
                }
                break;
            case 's':
                {
                    std::string str = argv[++i];
                    stepSize = boost::lexical_cast<int>(str);
                }
                break;
            case 'r':
                {
                    std::string str = argv[++i];
                    amountOfSimulationsToPerform = boost::lexical_cast<int>(str);
                }
                break;
            case 'c':
                {
                    try 
                    {
                        std::string filepath = argv[++i];
                        std::ifstream file;
                        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                        file.open(filepath);
                        file >> config; // read the config
                        std::cout << "loaded config:\n" << config << "\n";
                    } 
                    catch(boost::bad_lexical_cast& e)
                    {
                        std::cerr << e.what();
                        std::exit(5);
                    }
                    catch(std::exception& e)
                    {
                        std::cerr << e.what();
                        std::exit(6);
                    }
                }
                break;
            default:
                std::cout << "Invalid flag\n";
                printValidFlags();
                std::exit(-4);
                break;
        }
    }
}

namespace 
{
std::string obtainValue(const std::string& line)
{
    return std::string(line.begin() + line.find_last_of('=') + 1, line.end());
}

template <class T>
void read(std::string& buffer, std::istream& is, T& value)
{
    std::getline(is, buffer);
    auto stringValue = obtainValue(buffer);
    stringValue.erase(std::remove_if(stringValue.begin(), stringValue.end(), [](char c) { return std::isspace(c); }), stringValue.end());
    value = boost::lexical_cast<T>(stringValue);
}
}


std::istream& operator>>(std::istream& is, SymbolicRegressionSolver::Config& config)
{
    std::string buffer;

    read(buffer, is, config.initialPopulation);
    read(buffer, is, config.maxGenerations);
    read(buffer, is, config.initialMaxDepth);
    read(buffer, is, config.maxSolutionDepth);
    read(buffer, is, config.keepPercentage);
    read(buffer, is, config.mutationPercent);
    read(buffer, is, config.matePercent);

    read(buffer, is, config.generateConstantNodes);

    // have to do this separately for const dist
    int a, b;
    read(buffer, is, a);
    read(buffer, is, b);

    config.constantDist = decltype(config.constantDist){a, b};

    read(buffer, is, config.solutionCriterea);
    read(buffer, is, config.chanceToChangeConstant);
    read(buffer, is, config.chanceToChangeVar);
    
    int nearestNeighbour = 0;
    read(buffer, is, nearestNeighbour);
    config.nearestNeighbourOption = static_cast<decltype(config.nearestNeighbourOption)>(nearestNeighbour);

    read(buffer, is, config.chanceToUseNearestNeighbour);
    read(buffer, is, config.stepSize);
    
    int refillOption = 0;
    read(buffer, is, refillOption);
    config.populationRefillOption = static_cast<decltype(config.populationRefillOption)>(refillOption);
    
    return is;
}
