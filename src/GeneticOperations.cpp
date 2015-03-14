#include "GeneticOperations.hpp"

#include "lang/Node.hpp"

template <class F>
void visit_const(const NodePtr& node, F f)
{
    if(node->type() != Node::Type::OPERATION)
    {
        f(node);
        return;
    }

    const OperatorNode& operatorNode = *static_cast<const OperatorNode*>(node.get());
    f(operatorNode.left);
    f(operatorNode.right);
}

template <class F>
void visit(NodePtr& node, F f)
{
    if(node->type() != Node::Type::OPERATION)
    {
        f(node);
        return;
    }

    OperatorNode& operatorNode = *static_cast<OperatorNode*>(node.get());
    f(operatorNode.left);
    f(operatorNode.right);
}

int depth(const NodePtr& node)
{
    // note: I could write this in terms on visit_const,
    // however I feel like that's unnecessary overhead for the compiler to optimize
    // also it makes the code a bit more un-readable
    if(node->type() != Node::Type::OPERATION)
    {
        return 1;
    }

    OperatorNode& operatorNode = *static_cast<OperatorNode*>(node.get());
    int leftDepth = depth(operatorNode.left);
    int rightDepth = depth(operatorNode.right);
    return 1 + leftDepth > rightDepth ? leftDepth  : rightDepth ;
}

int node_count(const NodePtr& node)
{
    int count = 0;
    visit_const(node, [&](const NodePtr&) { ++count; });
    return count;
}

Function& mutate(RandomEngine& engine, Function& fn, std::uniform_int_distribution<> constantDist)
{
    static std::uniform_int_distribution<> operatorDist(0, (int)Operator::COUNT);

    const int NODE_COUNT = node_count(fn.getNode());
    const int DEPTH_OF_AST = depth(fn.getNode());
    const int amountOfNodesToChange = std::uniform_int_distribution<>(1, NODE_COUNT)(engine);

    std::vector<int> nodesToChange(amountOfNodesToChange, -1);
    std::uniform_int_distribution<> dist(0, NODE_COUNT);
    for(auto node : nodesToChange)
    {
        do 
        {
            // randomly generate a UNIQUE node to change
            node = dist(engine);
        } while (std::find(nodesToChange.begin(), nodesToChange.end(), node) == std::end(nodesToChange));
    }

    int currentNode = -1;
    visit(fn.getNode(), [&](NodePtr& node)
            {
                ++currentNode;
                // if we shouldn't change this node... 
                if(std::find(nodesToChange.begin(), nodesToChange.end(), currentNode) == std::end(nodesToChange))
                {
                    return; // then don't
                }
                
                // now we shall actually mutate the node
                switch(node->type())
                {
                // change the operator type
                case Node::Type::OPERATION:
                    static_cast<OperatorNode*>(node.get())->op = static_cast<Operator>(operatorDist(engine));
                    break;
                case Node::Type::VALUE:
                    static_cast<ValueNode*>(node.get())->value = constantDist(engine);
                    break;
                case Node::Type::VARIABLE:
                    // change the variable to a constant? TODO
                    // maybe make this a bool flag. Also ISSUE:
                    // since I'm using shared_ptr's it's not guaranteed to 
                    // actually delete the reference. So in that sense, 
                    // we should be using unique_ptr's for the nodes. Since
                    // I know that no one else is going to be holding the Node.
                    break;
                }
            });
    return fn;
}

Function mate(RandomEngine& engine, const Function& p1, const Function& p2)
{
    return Function("TODO");
}
