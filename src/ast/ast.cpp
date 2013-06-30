#include "ast.hpp"


namespace sphingid
{
  namespace ast
  {

//------------------------------------------------------------------------------
// class Node
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// class Root
//
//------------------------------------------------------------------------------

    RootNode::RootNode(std::vector<Node*> next) : next_(next) {}

    RootNode::~RootNode()
    {
      each (i, next_) {
        delete (*i);
      }
    }

    std::string RootNode::str(void)
    {
      std::string s;
      each (i, next_) {
        s += (*i)->str() + "\n";
      }
      return s;
    }
    
//------------------------------------------------------------------------------
// class ExpNode
//
//------------------------------------------------------------------------------

    ExpNode::ExpNode(){}

    ExpNode::~ExpNode(){}

    bool ExpNode::isLvalue(void)
    {
      return false;
    }

    bool ExpNode::isAssignable(void)
    {
      return false;
    }

    bool ExpNode::isConst(void)
    {
      return false;
    }

//------------------------------------------------------------------------------
// class NumLiteralNode
//
//------------------------------------------------------------------------------

    NumLiteralNode::NumLiteralNode(int num) : num_(num) {}

    NumLiteralNode::~NumLiteralNode(){}

    bool NumLiteralNode::isAssignable(void)
    {
      return true;
    }

    bool NumLiteralNode::isConst(void)
    {
      return false;
    }

    bool NumLiteralNode::isLvalue(void)
    {
      return false;
    }

    int NumLiteralNode::allocSize(void)
    {
      return sizeof(num_);
    }

    std::string NumLiteralNode::str(void)
    {
      char buff[20];
      sprintf(buff, "%d", num_);
      return "(" + std::string(buff) + ")";
    }

//------------------------------------------------------------------------------
// class BinaryOpNode
//
//------------------------------------------------------------------------------

    BinaryOpNode::BinaryOpNode(std::string op, ExpNode* left, ExpNode* right)
      : op_(op), left_(left), right_(right) {}

    BinaryOpNode::~BinaryOpNode()
    {
      if (left_) delete left_;
      if (right_) delete right_;
    }

    int BinaryOpNode::allocSize(void)
    {
      return left_->allocSize();
    }

    std::string BinaryOpNode::str(void)
    {
      return "(" + left_->str() + " " + op_ + " " + right_->str() + ")";
    }


//------------------------------------------------------------------------------
// class TermSymbolNode
//
//------------------------------------------------------------------------------

    TermSymbolNode::TermSymbolNode(std::string s)
      : s_(s) {}

    TermSymbolNode::~TermSymbolNode() {}

    std::string TermSymbolNode::str(void)
    {
      return s_;
    }

  }
}
