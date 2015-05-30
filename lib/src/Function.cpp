#include "Function.hpp"

#include <boost/lexical_cast/bad_lexical_cast.hpp>

std::ostream& operator<<(std::ostream& os, const Function& fn)
{
    os << reverse_parse(fn.getNode());
    return os;
}

std::istream& operator>>(std::istream& in, Function& fn)
{
    std::string buffer;
    std::getline(in, buffer);  

    try
    {
        fn = parse(buffer).statement;
    }
    catch(boost::bad_lexical_cast& e)
    {
        in.setstate(std::istream::failbit); 
    }

    return in;
}
