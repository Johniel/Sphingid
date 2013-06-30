#ifndef _AST_AST_HPP_
#define _AST_AST_HPP_

#include <string>
#include <vector>

#include <cstdio>

#include "../parser/lexer.hpp"
#include "../macro.hpp"

namespace sphingid
{
  namespace ast
  {
//------------------------------------------------------------------------------
    class Node
    {
    public:
      Node() {}
      virtual ~Node() {}
      virtual std::string str(void) = 0;
      static Node* make(std::vector<Node*> v)
      {
        if (v.empty()) return (Node*)NULL;
        return v[0];
      }
    };

//------------------------------------------------------------------------------
    class RootNode : public Node
    {
    public:
      RootNode(std::vector<Node*>);
      virtual ~RootNode();
      std::string str(void);
      static Node* make(std::vector<Node*> v)
      {
        return new RootNode(v);
      }
    private:
      std::vector<Node*> next_;
    };

//------------------------------------------------------------------------------
    class ExpNode : public Node {
    public:
      ExpNode();
      virtual ~ExpNode();
      virtual bool isAssignable(void);
      virtual bool isConst(void);
      virtual bool isLvalue(void);
      virtual int allocSize(void) = 0;
    };

//------------------------------------------------------------------------------
    class BinaryOpNode : public ExpNode
    {
    public:
      BinaryOpNode(std::string, ExpNode*, ExpNode*);
      virtual ~BinaryOpNode();
      int allocSize(void);
      std::string str(void);
      static Node* make(std::vector<Node*> v)
      {
        assert((int)v.size() == 3 && "Binary Oporator");
        return new BinaryOpNode(v[1]->str(), (ExpNode*)v[0], (ExpNode*)v[2]);
      }
    protected:
      std::string op_;
      ExpNode* left_;
      ExpNode* right_;
    };

//------------------------------------------------------------------------------
    class NumLiteralNode : public ExpNode
    {
    public:
      NumLiteralNode(int);
      virtual ~NumLiteralNode();
      bool isAssignable(void);
      bool isConst(void);
      bool isLvalue(void);
      int allocSize(void);
      std::string str(void);
    private:
      int num_;
    };

//------------------------------------------------------------------------------
    class TermSymbolNode : public Node {
    public:
      TermSymbolNode(std::string);
      virtual ~TermSymbolNode();
      std::string str(void);
    private:
      std::string s_;
    };
  }
}

#endif /* _AST_AST_HPP_ */
