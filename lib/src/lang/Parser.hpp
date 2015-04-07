#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

#include "Node.hpp"
#include "Variable.hpp"

/*
 * Example Usage:
 * (+ 3 (* 2 x)) == 3 + 2*x
 *
 * Note: There is no assignment via a string
 */

struct ParseResult
{
    ParseResult() { }
    
    ParseResult(const ParseResult& p) :
        statement(p.statement->clone()),
        vars(p.vars)
    {
    }
    
    ParseResult& operator=(const ParseResult& p)
    {
        if(this != &p)
        {
            statement = p.statement->clone();
            vars = p.vars;
        }
        return *this;
    }
    
    ParseResult(ParseResult&& p) :
        statement(std::move(p.statement)),
        vars(std::move(p.vars))
    {
        
    }
    
    ParseResult& operator=(ParseResult&& p)
    {
        if(this != &p)
        {
            statement = std::move(p.statement);
            vars = std::move(p.vars);
        }
        return *this;
    }

    
    
    NodePtr statement; // statement that was parsed
    VariableMap vars;  // the variables associated with the statement
};

ParseResult parse(std::string source);

std::string reverse_parse(const NodePtr& node);

#endif // PARSER_HPP
