#include <iostream>
#include <string>

#include "Point.hpp"
#include "Function.hpp"
#include "SymbolicRegressionSolver.hpp"

#include "string_utils.hpp"

SymbolicRegressionSolver::Config config{};
Function fn{"(* (* 5 x) (* 2 x))"};
int initialPoint = 0;
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
}

int main(int argc, char *argv[])
{
    parseArguments(argc, argv);

    std::cout << "function: " << fn << '\n';
    std::cout << "initial point: " << initialPoint << '\n';
    std::cout << "end point: " << endPoint << '\n';
    std::cout << "step size: " << stepSize << '\n';
    
    std::vector<Point> points;
    points.reserve((endPoint - initialPoint)/stepSize);

    for(int i = initialPoint; i <= endPoint; i += stepSize)
    {
        points.emplace_back(i, fn(i));
    }

    std::cout << "\npoints for function " << fn << ":\n";

    for(auto& p : points) 
    {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }

    SymbolicRegressionSolver solver(config, points);
    auto solutions = solver.solve();

    if(solutions.size() == 0)
    {
        std::cout << "No solution found!\n";
    }
    
    for(size_t i = 0; i < solutions.size(); ++i)
    {
        auto solution = solutions[i];
        std::cout << "solution " << i + 1 << ":\n";
        std::cout << "\tfunction: " << solution.function << '\n';
        std::cout << "\tfitness: " << solution.fitnessLevel << '\n';
        std::cout << "\tnew?: " << solution.newSolution << '\n';

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
