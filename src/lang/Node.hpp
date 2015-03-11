#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <utility>

#include "../Assert.hpp"

#include "Operator.hpp"
#include "Variable.hpp"

struct Node
{
    enum class Type
    {
        VALUE,
        VARIABLE,
        OPERATION
    };

    virtual Type type() const = 0;
    
private:

    virtual int eval(const VariableMap* map) const = 0;
    friend int compute(Node&, const VariableMap*);
};

using NodePtr = std::shared_ptr<Node>;

template <class T, class... Args>
NodePtr node(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

struct ValueNode : public Node
{
    int value;
    ValueNode(int value) :
        value(value)
    {
    }

    virtual Type type() const override
    {
        return Type::VALUE;
    }

private:

    virtual int eval(const VariableMap* map) const override
    {
        return value;
    }
};

struct OperatorNode : public Node
{
    Operator op;
    NodePtr left;
    NodePtr right;

    OperatorNode(Operator op, NodePtr left, NodePtr right) :
        op(op), left(std::move(left)), right(std::move(right))
    {
    }

    virtual Type type() const override
    {
        return Type::OPERATION;
    }

private:

    virtual int eval(const VariableMap* map) const override
    {
        return ::eval(compute(*left, map), compute(*right, map), op); 
    }
};

struct VariableNode : public Node
{
    Variable id;

    VariableNode(const Variable& id) : id(id) {}

    virtual Type type() const override
    {
        return Type::VARIABLE;
    }

private:

    virtual int eval(const VariableMap* map) const override
    {
        return map != nullptr && map->find(id) == map->end() ? 0 : map->at(id);
    }
};


inline int compute(Node& node, const VariableMap* map)
{
    return node.eval(map);
}


#endif // NODE_HPP
