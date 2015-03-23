#include "GeneticOperations.hpp"

#include <queue>

#include "lang/Node.hpp"

template <class F>
void visit(const NodePtr& node, F f)
{
    if(node->type() != Node::Type::OPERATION)
    {
        f(node);
        return;
    }

    const OperatorNode& operatorNode = *static_cast<const OperatorNode*>(node.get());
    f(node);
    visit(operatorNode.left, f);
    visit(operatorNode.right, f);
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
    f(node);
    visit(operatorNode.left, f);
    visit(operatorNode.right, f);
}

int depth(const NodePtr& node);

int nodeCount(const NodePtr& node);

std::vector<int> randomNodes(RandomEngine& engine, const NodePtr& node);

Function& mutate(RandomEngine& engine, Function& fn, std::uniform_int_distribution<> constantDist)
{
    static std::uniform_int_distribution<> operatorDist(0, (int)Operator::COUNT - 1);

    auto nodesToChange = randomNodes(engine, fn.getNode());

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
                        {
                            
                            std::uniform_int_distribution<> temp(constantDist.param().a(), constantDist.param().b() + 1);
                            auto number = temp(engine);
                            if(number == constantDist.param().b() + 1)
                            {
                                node = ::node<VariableNode>("x");
                            }
                            else
                            {
                                static_cast<ValueNode*>(node.get())->value = constantDist(engine);
                            }
                        }
                        break;
                    case Node::Type::VARIABLE:
                        // change the variable to a constant? TODO
                        // maybe make this a bool flag. Also ISSUE:
                        // since I'm using shared_ptr's it's not guaranteed to
                        // actually delete the reference. So in that sense,
                        // we should be using unique_ptr's for the nodes. Since
                        // I know that no one else is going to be holding the Node.
                        node = ::node<ValueNode>(constantDist(engine));
                    break;
                    default:
                        break;
                }
            });
    return fn;
}

Function mate(RandomEngine& engine, const Function& p1, const Function& p2)
{
    const Function* parents[] = { &p1, &p2 };
    std::vector<int> nodesToChange[] = { randomNodes(engine, p1.getNode()), randomNodes(engine, p2.getNode()) };

    std::queue<NodePtr> singleNodes;
    std::vector<NodePtr> operatorNodes;

    NodePtr rootNode;

    for(int i = 0; i < 2; ++i)
    {
        auto& listOfNodes = nodesToChange[i];
        auto& parent = *parents[i];
        int currentNode = -1;

        visit(parent.getNode(), [&](NodePtr& node)
                {
                    ++currentNode;
                    if(std::find(listOfNodes.begin(), listOfNodes.end(), currentNode) == std::end(listOfNodes))
                    {
                        return; 
                    }

                    // ughhhhhhh TODO
                    switch(node->type())
                    {
                        case Node::Type::OPERATION:
                            if(singleNodes.size() >= 2)
                            {
                                 
                            } 
                            else
                            {
                                operatorNodes.push_back(node); 
                            }
                            break;
                        case Node::Type::VARIABLE:
                        case Node::Type::VALUE:
                            singleNodes.push(node);

                            for(auto& n : operatorNodes)
                            {
                                auto op = static_cast<OperatorNode*>(n.get());
                                if(op->left == nullptr)
                                {
                                    auto& nodeToPutIn = singleNodes.front();
                                    singleNodes.pop();
                                    op->left = nodeToPutIn;
                                    break;
                                }

                                if(op->right == nullptr)
                                {
                                    auto& nodeToPutIn = singleNodes.front();
                                    singleNodes.pop();
                                    op->right = nodeToPutIn;
                                    break;
                                }
                            }

                            break;
                        default:
                            assert(0 && "Oops. I dunno how this even occurred");
                            break;
                    }
                }
        );
    }

    // if we pick zero operator nodes then we shall create an operator node
    // if we pick one operator node, then we shall choose half of the operator node (50:50)
    // if we pick two operator nodes then we shall do the above but using one of the two operators (50:50)
    
    return Function(rootNode);
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

int nodeCount(const NodePtr& node)
{
    int count = 0;
    visit(node, [&](const NodePtr&) { ++count; });
    return count;
}

std::vector<int> randomNodes(RandomEngine& engine, const NodePtr& node)
{
    const int NODE_COUNT = nodeCount(node);
    const int amountOfNodesToChange = std::uniform_int_distribution<>(1, NODE_COUNT)(engine);

    std::vector<int> nodes(amountOfNodesToChange, -1);
    std::uniform_int_distribution<> dist(0, NODE_COUNT);
    for(auto& node : nodes)
    {
        int newNode;
        do
        {
            newNode = dist(engine);
        }
        while (std::find(nodes.begin(), nodes.end(), node) == std::end(nodes));
        
        node = newNode;
    }
    return nodes;
}
