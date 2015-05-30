#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "lang/Node.hpp"
#include "lang/Parser.hpp"

class Function
{
public:

    Function(VariableName variableName = "x") :
        m_variableName{std::move(variableName)}
    {
    }

    Function(NodePtr node, VariableName variableName = "x") :
        Function{std::move(variableName)}
    {
        *this = std::move(node);
    }
    
    Function(const Function& fn) : 
        m_variableName{fn.m_variableName},
        m_node{fn.m_node != nullptr ? fn.m_node->clone().release() : nullptr}
    {
    }

    Function& operator=(const Function& fn)
    {
        m_node.reset(fn.m_node != nullptr ? fn.m_node->clone().release() : nullptr);
        m_variableName = fn.m_variableName;
        return *this;
    }

    Function(Function&&) noexcept = default;
    Function& operator=(Function&&) noexcept = default;

    Value operator()(VariableMap& map, Value varValue) const
    {
        map[m_variableName] = varValue;
        return getNode()->eval(&map);
    }

    NodePtr& getNode() { return m_node; }
    const NodePtr& getNode() const { return m_node; }

    Function& operator=(NodePtr node) 
    { 
        ASSERT(node != nullptr, "node is null"); 
        m_node = std::move(node); 
        return *this; 
    }

private:

    VariableName m_variableName;
    NodePtr m_node;
};

std::ostream& operator<<(std::ostream&, const Function&);
std::istream& operator>>(std::istream&, Function&);

#endif // FUNCTION_HPP
