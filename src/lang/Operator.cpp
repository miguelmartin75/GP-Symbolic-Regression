#include "Operator.hpp"
#include <functional>

static const std::function<int(int, int)> FUNCTION_MAP[] = 
{
    { [](int x1, int x2) -> int { return x1 + x2; } },
    { [](int x1, int x2) -> int { return x1 - x2; } },
    { [](int x1, int x2) -> int { return x1 * x2; } },
    //{ [](int x1, int x2) -> int { return x1 / x2; } },
    { [](int x1, int x2) -> int { return x2 == 0 ? 0 : x1 / x2; } },
};

int eval(int x1, int x2, Operator op) 
{
    return FUNCTION_MAP[static_cast<size_t>(op)](x1, x2);
}

std::istream& operator>>(std::istream& in, Operator& op)
{
    std::string buffer;
    in >> buffer;

    if(buffer.empty()) return in;

    switch(buffer[0])
    {
        case '+':
            op = Operator::ADD;
            break;
        case '-':
            op = Operator::SUBTRACT;
            break;
        case '*':
            op = Operator::MULTIPLY;
            break;
        case '/':
            {
                if(buffer.size() >= 2 && buffer[1] == '/')
                {
                    op = Operator::DIVIDE_SAFE;
                }
                /*
                else
                {
                    op = Operator::DIVIDE;
                }*/
            }
            break;
        default:
            break;
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const Operator& op)
{
    constexpr const char* STRINGS[] = { "+", "-", "*", "/", "//" };
    out << STRINGS[static_cast<size_t>(op)];
    return out;
}
