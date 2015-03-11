#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <vector>
#include <string>
// TODO
//#include "string_view.hpp"

struct Token
{
    enum class Type
    {
        NUMBER,
        VARIABLE,
        OPERATOR,
        LEFT_BRAC,
        RIGHT_BRAC
    } type;

    std::string source;

    Token(Type type, const std::string& source) :
        type(type), source(source)
    {
    }

//    string_view source;
};

using TokenList = std::vector<Token>;

#endif // TOKEN_HPP
