#include "Function.hpp"

std::ostream& operator<<(std::ostream& os, const Function& fn)
{
    os << reverse_parse(fn.getNode());
    return os;
}

std::istream& operator>>(std::istream& in, Function& fn)
{
    std::string buffer;
    std::getline(in, buffer);  
    fn = parse(buffer).statement;
    return in;
}
