#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
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

    SymbolicRegressionSolver solver{config, points};

    enum Direction { HIGHER, LOWER } dir = LOWER;

    auto& variableToModify = solver.config().mutationPercent;
    auto bestConfigOption = variableToModify;
    double bestAverage;
    size_t bestTotalSolutions = 0;

    {
        auto solutions = solver.solve();
        bestAverage = solver.currentGeneration();
        bestTotalSolutions = solutions.size();
        solver.reset();
    }

    variableToModify /= 2;

    while(std::abs(variableToModify - bestConfigOption) > 0.001)
    {
        /*
        std::cout << "bestConfigOpt = " << bestConfigOption << '\n';
        std::cout << "variable to modify = " << variableToModify << '\n';
        */

        double currentAverage = 0;
        size_t totalSolutions = 0;

        for(int i = 1; i < amountOfSimulationsToPerform; ++i)
        {
            auto solutions = solver.solve();
            totalSolutions += solutions.size();

            currentAverage += solver.currentGeneration();

#ifndef OPTIMISE_CONFIG
            for(auto& solution : solutions)
            {
                /*
                result.averageGen += solver.currentGeneration();
                result.minGen = result.minGen == -1 ? solver.currentGeneration() : std::min(result.minGen, solver.currentGeneration());
                result.maxGen = result.maxGen == -1 ? solver.currentGeneration() : std::max(result.maxGen, solver.currentGeneration());
                */

                std::cout << solver.currentGeneration() << ",";
                std::cout << solution.fitnessLevel << ",";
                std::cout << solution.mutated << ","; 
                std::cout << solution.mated << '\n';
            }
#endif // OPTIMISE_CONFIG

            solver.reset();
        }
        
        currentAverage /= totalSolutions;
        /*
        std::cout << "currentAverage= " << currentAverage << '\n';
        std::cout << "bestAverage= " << bestAverage << '\n';
        std::cout << "total solutions= " << totalSolutions << '\n';
        std::cout << "best total solutions= " << bestTotalSolutions << '\n';
        */
        if((totalSolutions > bestTotalSolutions &&
            std::abs(static_cast<int>(totalSolutions - bestTotalSolutions)) >= amountOfSimulationsToPerform / 10.0) ||
            currentAverage <= bestAverage)
        {
            bestAverage = currentAverage;
            bestTotalSolutions = totalSolutions;
            bestConfigOption = variableToModify;
            //dir = LOWER;
            //std::cout << "[IMPORTANT]: beat previous best\n";
        }
        else
        {
            dir = static_cast<Direction>(!static_cast<bool>(dir));
            //dir = HIGHER;
        }
        //std::cout << "\n\n";
        
        if(dir == HIGHER)
        {
            variableToModify = (bestConfigOption + variableToModify) / 2;
        }
        else
        {
            variableToModify /= 2;
        }
    }

    variableToModify = bestConfigOption;

    std::cout << "optimal var: " << bestConfigOption << '\n';
    /*
    std::cout << "optimal config is: \n";
    std::cout << solver.config() << '\n';
    */

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
