#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

#include "Token.hpp"

struct LexerResult
{
    TokenList tokens;

    enum class Error
    {
        SUCCESS,
        MIS_MATCHED_BRACES,
        INVALID_OPERATOR
    } error = Error::SUCCESS;
};

LexerResult tokenize(std::string source);

#endif // LEXER_HPP
