#include "Lexer.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

// NOTE: only support
// single letter var names for now

bool is_operator(const std::string& str);

LexerResult tokenize(std::string source)
{
    LexerResult result;
    auto& tokens = result.tokens;

    std::istringstream stream{source};
    std::string buffer;

    int bracketAccumulator = 0;

    while(stream >> buffer)
    {
        for(auto it = buffer.begin(); it != buffer.end(); ++it)
        {
            char ch = *it;
            if(ch == '(')
            {
                bracketAccumulator++;
                continue;
            }
            else if(ch == ')')
            {
                bracketAccumulator--;
                continue;
            }
            
            auto itToEndBracket = std::find(it, buffer.end(), ')');
            auto itToLogicalEnd = itToEndBracket == buffer.end() ? buffer.end() : itToEndBracket;
            if(itToLogicalEnd == buffer.end())
            {
                bracketAccumulator--;
            }
            
            std::string text(it, itToLogicalEnd);

            if(std::isdigit(ch))
            {
                tokens.emplace_back(Token::Type::NUMBER, text);
                break;
            }
            else if(std::isalpha(ch))
            {
                tokens.emplace_back(Token::Type::VARIABLE, text);
                break;
            }
            else // operator
            {
                // TODO INVALID OPERATOR ERROR
                if(is_operator(text))
                    tokens.emplace_back(Token::Type::OPERATOR, text);
                else
                {
                    std::cerr << "invalid operator: " << text << '\n';
                    std::exit(-1);
                }
            }
        }
    }

    if(bracketAccumulator != 0)
        result.error = LexerResult::Error::MIS_MATCHED_BRACES;

    return result;
}

bool is_operator(const std::string& str)
{
    return str == "+" || str == "-" || str == "*" || str == "/" || str == "//";
}
