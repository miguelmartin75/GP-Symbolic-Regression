#include "GeneticOperations.hpp"

#include <queue>

#include "lang/Node.hpp"
#include "util.hpp"

// note: if it can't append the node,
// it will join it with a RANDOM operator node
void add(RandomEngine& engine, NodePtr& root, NodePtr nodeToInsert);

template <class F>
void visit(const NodePtr& node, F f)
{
    ASSERT(node.get(), "node is null");
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
    ASSERT(node.get(), "node is null");
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

void mutate(RandomEngine& engine, NodePtr& fn, std::uniform_int_distribution<> constantDist, double chanceToChangeVar, double chanceToChangeConstant, bool useNearestNeighbour, int stepSize)
{
    static std::uniform_int_distribution<> operatorDist(0, (int)Operator::COUNT - 1);

    auto nodesToChange = randomNodes(engine, fn);
    std::uniform_int_distribution<> changeVarDist(0, chanceToChangeVar);
    std::uniform_int_distribution<> changeConstantDist(0, chanceToChangeConstant);

    ASSERT(fn != nullptr, "function is null");
    
    int currentNode = -1;
    visit(fn, [&](NodePtr& node)
            {
                ++currentNode;
                // if we shouldn't change this node...
                if(!util::contains(nodesToChange, currentNode))
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
                            if(changeConstantDist(engine))
                            {
                                node = ::node<VariableNode>("x");
                            }
                            else
                            {
                                ValueNode& valueNode = *static_cast<ValueNode*>(node.get());
                                if(useNearestNeighbour)
                                {
                                    int sign = std::uniform_int_distribution<>{0, 1}(engine) ? 1 : -1;
                                    valueNode.value += sign * stepSize;
                                }
                                else
                                {
                                    valueNode.value = constantDist(engine);
                                }
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
                        if(changeVarDist(engine)) 
                        {
                            node = ::node<ValueNode>(constantDist(engine));
                        }
                    break;
                    default:
                        break;
                }
            });
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
    std::uniform_int_distribution<> dist(0, NODE_COUNT - 1);
    for(auto& node : nodes)
    {
        int newNode;
        do
        {
            newNode = dist(engine);
        }
        while (util::contains(nodes, newNode));
        
        node = newNode;
    }
    return nodes;
}

NodePtr mate(RandomEngine& engine, const NodePtr& p1, const NodePtr& p2, std::function<NodePtr()> nodeCreator, int maxDepth)
{
    const NodePtr *const parents[] = { &p1, &p2 };
    std::vector<int> nodesToChange[] = { randomNodes(engine, p1), randomNodes(engine, p2) };

    NodePtr rootNode = nullptr;
    std::vector<NodePtr> operators;
    std::vector<NodePtr> singles;

    for(int i = 0; i < 2; ++i)
    {
        auto& listOfNodes = nodesToChange[i];
        auto& parent = *parents[i];
        int currentNode = -1;

        visit(parent, [&](const NodePtr& node)
                {
                    ++currentNode;
                    if(!util::contains(listOfNodes, currentNode))
                    {
                        return;
                    }

                    switch(node->type())
                    {
                        case Node::Type::OPERATION:
                            operators.emplace_back(::node<OperatorNode>(static_cast<OperatorNode*>(node.get())->op, nullptr, nullptr));
                            break;
                        case Node::Type::VARIABLE:
                        case Node::Type::VALUE:
                            singles.emplace_back(node->clone());
                            break;
                        default:
                            assert(0 && "Oops. I dunno how this even occurred");
                            break;
                    }
                }
        );
    }
    
    for(auto& op : operators)
    {
        NodePtr left;
        NodePtr right;
        if(singles.size() < 2)
        {
            std::uniform_int_distribution<> dist(0, 1);

            size_t amountOfSinglesToMake = 2 - singles.size();
            if(amountOfSinglesToMake == 2 && dist(engine) && rootNode != nullptr)
            {
                // just drop the node...
                break;
            }
            else if(amountOfSinglesToMake == 1)
            {
                left = singles.front()->clone();
                right = nodeCreator();
                singles.erase(singles.begin());
            }
            else
            {
                left = nodeCreator();
                right = nodeCreator();
            }
        }
        else
        {
            left = std::move((*singles.begin())->clone());
            right = std::move((*(singles.begin() + 1))->clone());
            // erase does [first, last)
            singles.erase(singles.begin(), singles.begin() + 2);
        }
        
        add(engine, rootNode, std::move(op->clone()));
        if(left)
            add(engine, rootNode, std::move(left->clone()));
        if(right)
            add(engine, rootNode, std::move(right->clone()));
    }
    
    // just add the rest of the singles with random op nodes
    for(auto& si : singles)
    {
        add(engine, rootNode, std::move(si->clone()));
    }

    // because I'm lazy...
    // TODO
    // trim(rootNode, maxDepth);
    
    return rootNode;
}

bool recursive_append(RandomEngine& engine, NodePtr& root, NodePtr&& nodeToInsert)
{
    static std::uniform_int_distribution<> dist(0, 1);
    // if we have an empty tree
    if(root.get() == nullptr)
    {
        root.reset(nodeToInsert.release());
        return true;
    }
    else if(root->type() == Node::Type::OPERATION)
    {
        OperatorNode& opNode = *static_cast<OperatorNode*>(root.get());
        bool useLeft = dist(engine);
        NodePtr& left = useLeft ? opNode.left : opNode.right;
        NodePtr& right = useLeft ? opNode.right : opNode.left;
        
        // try the root node FIRST
        if(left.get() == nullptr)
        {
            left.reset(nodeToInsert.release());
            return true;
        }
        else if(right.get() == nullptr)
        {
            right.reset(nodeToInsert.release());
            return true;
        }
        else
        {
            // then recurse down the tree
            return recursive_append(engine, left, std::move(nodeToInsert)) || recursive_append(engine, right, std::move(nodeToInsert));
        }
    }

    return false;
}

void add(RandomEngine& engine, NodePtr& root, NodePtr nodeToInsert)
{
    static std::uniform_int_distribution<> opDist(0, (int)Operator::COUNT - 1);
    
    if(recursive_append(engine, root, std::move(nodeToInsert)))
    {
        return;
    }

    auto operatorToUse = (Operator)opDist(engine);
    root = node<OperatorNode>(operatorToUse, std::move(root), std::move(nodeToInsert));
}
