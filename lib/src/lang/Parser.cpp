#include "Parser.hpp"

#include <stack>
#include <queue>

#include <boost/lexical_cast.hpp>

#include "Lexer.hpp"
#include "Node.hpp"

enum class NodeType
{
    UNDEFINED,
    OPERATOR,
    VALUE
};

struct NodeData
{
    NodePtr node;
    TokenList::iterator iteratorToGetNode;
};

NodeData nextNode(TokenList::iterator it, TokenList::iterator end, VariableMap& map);

ParseResult parse(std::string source)
{
    ParseResult result;
    auto tokens = tokenize(std::move(source)).tokens;
    result.statement = nextNode(tokens.begin(), tokens.end(), result.vars).node;

    return result;
}


NodeData nextNode(TokenList::iterator begin, TokenList::iterator end, VariableMap& map)
{
    NodeData data;
    for(; begin != end; ++begin)
    {
        auto& token = *begin;
        switch(token.type)
        {
            case Token::Type::RIGHT_BRAC:
            case Token::Type::LEFT_BRAC:
                continue;

            case Token::Type::OPERATOR:
                {
                    auto leftNodeData = nextNode(begin + 1, end, map);
                    auto rightNodeData = nextNode(leftNodeData.iteratorToGetNode + 1, end, map);
                    return NodeData{node<OperatorNode>(boost::lexical_cast<Operator>(token.source), std::move(leftNodeData.node), std::move(rightNodeData.node)),
                                    rightNodeData.iteratorToGetNode};
                }
                break;
            case Token::Type::NUMBER:
                {
                    return NodeData{node<ValueNode>(boost::lexical_cast<int>(token.source)), begin};
                }
                break;
            case Token::Type::VARIABLE:
                {
                    if(map.find(token.source) == map.end()) { map.emplace(token.source, int()); }
                    return NodeData{node<VariableNode>(token.source), begin};
                }
                break;
        }
    }
    
    return data;
}


std::string nextNode(const NodePtr& node)
{
    if(!node) return std::string{""};
    
    switch(node->type())
    {
        case Node::Type::OPERATION:
            {
                auto& castedNode = *static_cast<OperatorNode*>(node.get());
                if(castedNode.left == nullptr || castedNode.right == nullptr) 
                    return std::string{"null"};
                return "(" + boost::lexical_cast<std::string>(castedNode.op) + " " +
                        nextNode(castedNode.left) + " " + nextNode(castedNode.right) + ")";
            }
            break;
        case Node::Type::VALUE:
            {
                auto& castedNode = *static_cast<ValueNode*>(node.get());
                return boost::lexical_cast<std::string>(castedNode.value);
            }
           break;
        case Node::Type::VARIABLE:
            {
                auto& castedNode = *static_cast<VariableNode*>(node.get());
                return boost::lexical_cast<std::string>(castedNode.id);
            }
            break;
        default:
            return std::string{"invalid node"};
    }
}

std::string reverse_parse(const NodePtr& node)
{
    return nextNode(node);
}
