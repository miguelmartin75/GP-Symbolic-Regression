#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "lang/Node.hpp"
#include "lang/Parser.hpp"

class Function
{
public:

    Function() :
        m_variableName("x")
    {
    }

    Function(NodePtr node, const std::string& variableName = "x") :
        m_variableName(variableName)
    {
        *this = std::move(node);
    }

    Function(const std::string& source, const std::string& variableName = "x") :
        m_variableName(variableName)
    {
        *this = source;
    }
    
    Function(const Function&) = default;
    Function& operator=(const Function&) = default;
    Function(Function&&) = default;
    Function& operator=(Function&&) = default;

    int operator()(int variable) 
    {
        getVariableMap()[m_variableName] = variable;
        return getNode()->eval(&getVariableMap());
    }

    Function& operator=(const std::string& str)
    {
        parse(str);
        return *this;
    }

    Function& operator=(NodePtr node)
    {
        m_parserResult.statement = std::move(node);
        return *this;
    }

    VariableMap& getVariableMap()
    {
        return m_parserResult.vars;
    }

    const NodePtr& getNode() const
    {
        return m_parserResult.statement;
    }

    NodePtr& getNode() 
    {
        return m_parserResult.statement;
    }

private:

    void parse(std::string text)
    {
        m_parserResult = ::parse(text);
    }

    std::string m_variableName;
    ParseResult m_parserResult;
};

std::ostream& operator<<(std::ostream&, const Function&);
std::istream& operator>>(std::istream&, Function&);

#endif // FUNCTION_HPP
