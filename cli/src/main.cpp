#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "Point.hpp"
#include "Function.hpp"
#include "SymbolicRegressionSolver.hpp"
#include "Config.hpp"

//#define VERBOSE_LOG

int amountOfSimulationsToPerform = 1;
SymbolicRegressionSolver::Config config{};
Function fn{"(+ 1 x)"};
int initialPoint = -10;
int endPoint = 10;
int stepSize = 2;

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

void performSimulation(const PointList& points);

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

    for(int i = initialPoint; i <= endPoint; i += stepSize)
    {
        points.emplace_back(i, fn(i));
    }

    for(int i = 0; i < amountOfSimulationsToPerform; ++i)
    {
        performSimulation(points);
    }

    return 0;
}

void performSimulation(const PointList& points)
{
    SymbolicRegressionSolver solver(config, points);
    auto solutions = solver.solve();

    if(!solver.foundSolution())
    {
//        std::cout << "No solution!\n";
        return;
    }

    for(size_t i = 0; i < solutions.size(); ++i)
    {
        auto& solution = solutions[i];
        std::cout << solver.currentGeneration() << ",";
//        std::cout << i + 1 << ",";
        std::cout << solution.fitnessLevel << ",";
        std::cout << solution.mutated << ","; 
        std::cout << solution.mated << '\n';

#ifdef VERBOSE_LOG
           std::cout << "solution " << i + 1 << ":\n";
           std::cout << "\tfunction: " << solution.function << '\n';
           std::cout << "\tfitness: " << solution.fitnessLevel << '\n';
           std::cout << "\tmutated?: " << std::boolalpha << solution.mutated << '\n';
           std::cout << "\tmated?: " << solution.mated << '\n';
#endif //VERBOSE_LOG
    }
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
                    fn = functionAsString;
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

                        /*
                        std::cout << "Loaded config:\n";
                        std::cout << config << std::endl;
                        */
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
