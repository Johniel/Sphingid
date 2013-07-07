#ifndef _AST_AST_HPP_
#define _AST_AST_HPP_

#include <string>
#include <vector>

#include <cstdio>

#include "../parser/lexer.hpp"
#include "../macro.hpp"

#define TEST

#ifdef TEST
using namespace std;
#endif

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
#ifdef TEST
        std::cout << "Node::make" << std::endl;
        for (int i = 0; i < v.size(); ++i) {
          std::cout << "Node> " << v[i]->str() << std::endl;
        }
#endif
        if (v.empty()) return (Node*)NULL;
        return v[0];
      }
    };


//------------------------------------------------------------------------------
    class ArrayNode : public Node
    {
    public:
      ArrayNode() ;
      ArrayNode(std::vector<Node*>) ;
      virtual ~ArrayNode() ;
      std::string str(void) ;
      Node* operator [] (int) ;
      Node* nth(int i) { return array_[i]; }
      size_t size(void) { return array_.size(); }
      static Node* make(std::vector<Node*> v)
      {
        cout << "ArrayNode::make" << endl;
        for (int i = 0; i < v.size(); ++i) {
          cout << v[i]->str() << endl;
        }
        return new ArrayNode(v);
      }
      template<class T = Node>
      std::vector<T*> vec()
      {
        std::vector<T*> v;
        for (size_t i = 0; i < array_.size(); ++i) {
          v.push_back((T*)array_[i]);
        }
        return v;
      }
    private:
      std::vector<Node*> array_;
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
#ifdef TEST
        std::cout << "BinaryOpNode::make" << std::endl;
        for (int i = 0; i < v.size(); ++i) {
          std::cout << "BinaryOpNode>" << v[i]->str() << std::endl;
        }
#endif
        if (v.size() == 2) return new BinaryOpNode(v[1]->str(), (ExpNode*)v[0], (ExpNode*)v[2]);
        if (v.size() == 3) return new BinaryOpNode(v[0]->str(), (ExpNode*)v[0], (ExpNode*)v[2]);
        assert(false && "Binary Oporator");
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
    class StrLiteralNode : public ExpNode
    {
    public:
      StrLiteralNode(std::string);
      virtual ~StrLiteralNode();
      bool isAssignable(void);
      bool isConst(void);
      bool isLvalue(void);
      int allocSize(void);
      std::string str(void);
    private:
      std::string s_;
    };

//------------------------------------------------------------------------------
    class TermSymbolNode : public Node {
    public:
      TermSymbolNode(std::string);
      virtual ~TermSymbolNode();
      std::string str(void);
      static Node* make(std::vector<Node*> v)
      {
#ifdef TEST
        cout << "TermSymbolNode::make" << endl;
        for (int i = 0; i < v.size(); ++i) {
          cout << i << ": " << v[i]->str() << endl;
        }
#endif
        assert(v.size() == 1);
        return new TermSymbolNode(v[0]->str());
      }
    private:
      std::string s_;
    };

//------------------------------------------------------------------------------
    class StatNode : public Node {} ;

//------------------------------------------------------------------------------
    class FnDefNode : public StatNode {
    public:
      ~FnDefNode() ;
      virtual std::string str() ;
      static Node* make(std::vector<Node*> v)
      {
#ifdef TEST
        std::cout << "FnDefNode::make" << std::endl;
        for (int i = 0; i < v.size(); ++i) {
          cout << i << ": " << v[i]->str() << endl;
        }
#endif
        assert(v.size() == 4); // return_type, fn_name, arg_list, body
        return new FnDefNode(v);
      }
    private:
      FnDefNode(std::vector<Node*>) ;
      std::string name_;
      TermSymbolNode* ret_;
      std::vector< pair<TermSymbolNode*, string> > args_;
      RootNode* body_;
    };

//------------------------------------------------------------------------------
    class FnDeclNode : public StatNode {
    public:
      ~FnDeclNode() ;
      virtual std::string str() ;
      static Node* make(std::vector<Node*> v)
      {
#ifdef TEST
        std::cout << "FnDeclNode" << std::endl;
#endif
        return new FnDeclNode(v);
      }
    private:
      FnDeclNode(std::vector<Node*>) ;
      std::string name_;
      TermSymbolNode* ret_;
      std::vector<TermSymbolNode*> args_;
    } ;

//------------------------------------------------------------------------------
    class ClassNode : public StatNode {
    public:
      ClassNode() ;
      ~ClassNode() ;
      virtual std::string str() ;
      static Node* make(std::vector<Node*> v)
      {
#ifdef TEST
        std::cout << "ClassNode:make" << std::endl;
        for (int i = 0; i < v.size(); ++i) {
          cout << i << ": " << v[i]->str() << endl;
        }
#endif
        return new ClassNode(v);
      }
    private:
      ClassNode(std::vector<Node*>) ;
      std::string name_;
      std::vector<FnDeclNode*> fn_;
    } ;

//------------------------------------------------------------------------------
    class StructNode : public StatNode {
    public:
      StructNode() ;
      ~StructNode() ;
      virtual std::string str() ;
      static Node* make(std::vector<Node*> v)
      {
#ifdef TEST
        std::cout << "StructNode:make" << std::endl;
        for (int i = 0; i < v.size(); ++i) {
          cout << i << ": " << v[i]->str() << endl;
        }
#endif
        return new StructNode(v);
      }
    private:
      StructNode(std::vector<Node*>) ;
      std::string name_;
      std::vector< std::pair<std::string, std::string> > fn_;

    } ;

  }
}

#endif /* _AST_AST_HPP_ */
