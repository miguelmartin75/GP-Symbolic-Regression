#include <iostream>
#include <vector>
#include <string>

#include "Point.hpp"
#include "Function.hpp"
#include "SymbolicRegressionSolver.hpp"
#include "Config.hpp"

#include "string_utils.hpp"

//#define VERBOSE_LOG

int amountOfSimulationsToPerform = 1;
SymbolicRegressionSolver::Config config{};
Function fn{"(+ 1 x)"};
int initialPoint = -10;
int endPoint = 10;
int stepSize = 2;

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

    if(solutions.size() == 0)
    {
#ifdef VERBOSE_LOG
        std::cout << "No solution!\n";
#endif // VERBOSE_LOG
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
                    initialPoint = util::from_string<int>(str);
                }
                break;
            case 'e':
                {
                    std::string str = argv[++i];
                    endPoint = util::from_string<int>(str);
                }
                break;
            case 's':
                {
                    std::string str = argv[++i];
                    stepSize = util::from_string<int>(str);
                }
                break;
            case 'r':
                {
                    std::string str = argv[++i];
                    amountOfSimulationsToPerform = util::from_string<int>(str);
                }
                break;
            case 'c':
                std::cout << "TODO\n";
                std::exit(-3);
                break;
            default:
                std::cout << "Invalid flag\n";
                printValidFlags();
                std::exit(-4);
                break;
        }
    }
}
