#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include <iostream>

enum class Operator
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    //DIVIDE,
    DIVIDE_SAFE,

    // TODO:
    // cos, sin, etc.
    // and make it configurable to limit to a subset
    // of these operators

    COUNT
};

int eval(int x1, int x2, Operator op);

std::istream& operator>>(std::istream&, Operator&);
std::ostream& operator<<(std::ostream&, const Operator&);

#endif // OPERATOR_HPP
