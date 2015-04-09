#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <utility>

#include "../Assert.hpp"

#include "Operator.hpp"
#include "Variable.hpp"


struct Node;
using NodePtr = std::unique_ptr<Node>;

struct Node
{
    enum class Type
    {
        VALUE,
        VARIABLE,
        OPERATION,
        COUNT
    };

    virtual Type type() const = 0;
    virtual int eval(const VariableMap* map) const = 0;
    virtual NodePtr clone() const = 0;
    virtual ~Node() { }
};

template <class T, class... Args>
NodePtr node(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

struct ValueNode : public Node
{
    int value;
    ValueNode(int value) :
        value(value)
    {
    }

    virtual ~ValueNode() { }

    virtual Type type() const override
    {
        return Type::VALUE;
    }

    virtual int eval(const VariableMap* map) const override
    {
        return value;
    }
    
    virtual NodePtr clone() const { return node<ValueNode>(value); }
};

struct VariableNode : public Node
{
    Variable id;

    VariableNode(const Variable& id) : id(id) {}

    virtual ~VariableNode() { }

    virtual Type type() const override
    {
        return Type::VARIABLE;
    }

    virtual int eval(const VariableMap* map) const override
    {
        return map != nullptr && map->find(id) == map->end() ? 0 : map->at(id);
    }
    
    virtual NodePtr clone() const override { return node<VariableNode>(id); }
};


struct OperatorNode : public Node
{
    Operator op;
    NodePtr left;
    NodePtr right;

    OperatorNode(Operator op, NodePtr left, NodePtr right) :
        op{op}, left{std::move(left)}, right{std::move(right)}
    {
    }

    virtual ~OperatorNode() { }

    virtual Type type() const override
    {
        return Type::OPERATION;
    }

    virtual int eval(const VariableMap* map) const override
    {
        // special case for division and variable nodes. i.e. x/x == 1 for all x in real (including 0)
        if(op == Operator::DIVIDE_SAFE)/*|| op == Operator::DIVIDE)*/
        {
            if(left->type() == Node::Type::VARIABLE && right->type() == Node::Type::VARIABLE &&
               static_cast<VariableNode*>(left.get())->id == static_cast<VariableNode*>(right.get())->id)
            {
                return 1;
            }
        }

        return ::eval(left->eval(map), right->eval(map), op); 
    }
    
    virtual NodePtr clone() const override { return node<OperatorNode>(op, left == nullptr ? nullptr : std::move(left->clone()), right == nullptr ? nullptr : std::move(right->clone())); }
};

#endif // NODE_HPP
