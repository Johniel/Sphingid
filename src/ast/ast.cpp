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
        if (i != next_.begin()) s += "\n";
        s += (*i)->str();
      }
      return "(" + s + ")";
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
// class StrLiteralNode
//
//------------------------------------------------------------------------------

    StrLiteralNode::StrLiteralNode(std::string s) : s_(s) {}
    StrLiteralNode::~StrLiteralNode() {}
    bool StrLiteralNode::isAssignable(void) { assert(false); }
    bool StrLiteralNode::isConst(void) { assert(false); }
    bool StrLiteralNode::isLvalue(void) { assert(false); }
    int StrLiteralNode::allocSize(void) { assert(false); }
    std::string StrLiteralNode::str(void) { return "(" + s_ + ")"; }

//------------------------------------------------------------------------------
// class KeywordNode
//
//------------------------------------------------------------------------------

    KeywordNode::KeywordNode(std::string s) : s_(":" + s) {}
    KeywordNode::~KeywordNode() {}
    bool KeywordNode::isAssignable(void) { assert(false); }
    bool KeywordNode::isConst(void) { assert(false); }
    bool KeywordNode::isLvalue(void) { assert(false); }
    int KeywordNode::allocSize(void) { assert(false); }
    std::string KeywordNode::str(void) { return "(" + s_ + ")"; }


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

//------------------------------------------------------------------------------
// class ArrayNode
//
//------------------------------------------------------------------------------
    ArrayNode::ArrayNode() {}

    ArrayNode::ArrayNode(std::vector<Node*> v) : array_(v) {}

    ArrayNode::~ArrayNode() {}

    std::string ArrayNode::str(void)
    {
      std::string s;
      each(i, array_) s += (*i)->str() + ",";
      return "{" + s + "}";
    }

    Node* ArrayNode::operator [] (int idx)
    {
      return array_[idx];
    }


//------------------------------------------------------------------------------
// class StatNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// class FunDeclNode
//
//------------------------------------------------------------------------------

    FnDeclNode::FnDeclNode(std::vector<Node*> v)
    {
      this->ret_  = (TermSymbolNode*)v[0];
      this->name_ = v[1]->str();
      this->args_ = ((ArrayNode*)v[2])->vec<TermSymbolNode>();
    }

    FnDeclNode::~FnDeclNode()
    {
      if (ret_)  delete ret_;
      for (size_t i = 0; i < args_.size(); ++i) {
        delete args_[i];
      }
    }

    std::string FnDeclNode::str()
    {
      std::string s;
      s += this->name_;
      s += " :: ";
      for (size_t i = 0; i < args_.size(); ++i) {
        if (i) s += " -> ";
        s += args_[i]->str();
      }
      s += " -> " + this->ret_->str();
      return "(" + s + ")";
    }

//------------------------------------------------------------------------------
// class FnDefNode
//
//------------------------------------------------------------------------------

    FnDefNode::FnDefNode(std::vector<Node*> v)
    {
      this->ret_  = (TermSymbolNode*)v[0];
      this->name_ = v[1]->str();
      std::vector<ArrayNode*> args = ((ArrayNode*)v[2])->vec<ArrayNode>();
      for (size_t i = 0; i < args.size(); ++i) {
        this->args_.push_back(make_pair((TermSymbolNode*)args[i]->nth(0), args[i]->nth(1)->str()));
      }
      this->body_ = (RootNode*)v[3];
    }

    FnDefNode::~FnDefNode()
    {
      if (ret_)  delete ret_;
      for (size_t i = 0; i < args_.size(); ++i) {
        delete args_[i].first;
      }
    }

    std::string FnDefNode::str()
    {
      std::string s;
      s += this->name_;
      s += " :: ";
      for (size_t i = 0; i < args_.size(); ++i) {
        if (i) s += " -> ";
        s += args_[i].first->str() + "(" + args_[i].second + ")";
      }
      s += " -> " + this->ret_->str();
      s += "\n";
      s += this->body_->str();
      return "(" + s + ")";
    }

//------------------------------------------------------------------------------
// class ClassNode
//
//------------------------------------------------------------------------------

    ClassNode::ClassNode()
    {
      assert(false);
    }

    ClassNode::~ClassNode()
    {
    }

    std::string ClassNode::str()
    {
      std::string s;
      s += "class ";
      s += this->name_;
      for (size_t i = 0; i < fn_.size(); ++i) {
        s += "\n" + fn_[i]->str();
      }
      return "(" + s + ")";
    }

    ClassNode::ClassNode(std::vector<Node*> v)
    {
      this->name_ = v[0]->str();
      this->fn_ = ((ArrayNode*)v[1])->vec<FnDeclNode>();
    }

//------------------------------------------------------------------------------
// class StructNode
//
//------------------------------------------------------------------------------

    StructNode::StructNode()
    {
      assert(false);
    }

    StructNode::~StructNode() {}

    std::string StructNode::str()
    {
      std::string s;
      s += "struct ";
      s += this->name_;
      s += " : ";
      s += "[";
      for (size_t i = 0; i < klass_.size(); ++i) {
        s += klass_[i] + ", ";
      }
      s += "]";
      for (size_t i = 0; i < mem_.size(); ++i) {
        s += "\n";
        s += mem_[i]->str();
      }
      return "(" + s + ")";
    }

    StructNode::StructNode(std::vector<Node*> v)
    {
      int idx = 0;
      // v[idx] name
      this->name_ = v[idx]->str();
      ++idx;

      // v[idx] class
      if (v.size() == 3) {
        std::vector<Node*> w = ((ArrayNode*)v[1])->vec<Node>();
        for (size_t i = 0; i < w.size(); ++i) {
          this->klass_.push_back(w[i]->str());
        }
        ++idx;
      }

      // v[2] body
      this->mem_= ((ArrayNode*)v[idx])->vec<VarNode>();
      ++idx;
    }

//------------------------------------------------------------------------------
// class FnCallNode
//
//------------------------------------------------------------------------------


    FnCallNode::FnCallNode()
    {
      assert(false);
    }

    FnCallNode::~FnCallNode()
    {
    }

    bool FnCallNode::isAssignable(void) { return false; }
    bool FnCallNode::isConst(void) { return true; }
    bool FnCallNode::isLvalue(void) { return false; }
    int FnCallNode::allocSize(void) { return 0; }

    std::string FnCallNode::str()
    {
      std::string s;
      s += this->name_ + " ";
      for (size_t i = 0; i < this->args_.size(); ++i) {
        if (i) s += ", ";
        s += args_[i]->str();
      }
      return "(" + s + ")";
    }

    FnCallNode::FnCallNode(std::vector<Node*> v)
    {
      // v[0] : function name
      // v[1] : function args
      this->name_ = v[0]->str();
      if (1 < v.size()) {
        ArrayNode* args = (ArrayNode*)v[1];
        for (size_t i = 0; i < args->size(); ++i) {
          this->args_.push_back((ExpNode*)(args->nth(i)));
        }
      }
    }

//------------------------------------------------------------------------------
// class SelectionNode
//
//------------------------------------------------------------------------------

    SelectionNode::~SelectionNode()
    {
      if (this->cond_) delete this->cond_;
      if (this->then_) delete this->then_;
      if (this->else_) delete this->else_;
    }
    std::string SelectionNode::str()
    {
      std::string s;
      s += "if ";
      s += this->cond_->str();
      s += "\n";
      s += this->then_->str();
      if (this->else_) {
        s += "\n";
        s += this->else_->str();
      }
      return "(" + s + ")";
    }

    SelectionNode::SelectionNode(std::vector<Node*> v)
    {
      assert(2 <= v.size());
      // v[0] : cond
      // v[1] : then
      // v[2] : else ?
      this->cond_ = (ExpNode*)v[0];
      this->then_ = v[1];
      this->else_ = (v.size() == 3) ? v[2] : NULL;
    }

//------------------------------------------------------------------------------
// class WhileNode
//
//------------------------------------------------------------------------------

    WhileNode::~WhileNode()
    {
      if (this->cond_) delete this->cond_;
      if (this->body_) delete this->body_;
    }
    std::string WhileNode::str()
    {
      std::string s;
      s += "while ";
      s += this->cond_->str();
      s += "\n";
      s += this->body_->str();
      return "(" + s + ")";
    }

    WhileNode::WhileNode(std::vector<Node*> v)
    {
      assert(2 == v.size());
      // v[0] : cond
      // v[1] : body
      this->cond_ = (ExpNode*)v[0];
      this->body_ = v[1];
    }

//------------------------------------------------------------------------------
// class JumpNode
//
//------------------------------------------------------------------------------

    JumpNode::~JumpNode() {}

    std::string JumpNode::str()
    {
      std::string s;
      s += stat_;
      s += " ";
      if (this->val_) s += this->val_->str();
      return "(" + s + ")";
    }

    JumpNode::JumpNode(std::vector<Node*> v)
    {
      assert(v.size() <= 2);
      // v[0] : stat
      // v[1] : val
      this->stat_ = v[0]->str();
      this->val_ =  (2 <= v.size()) ? (ExpNode*)v[1] : NULL;
    }


//------------------------------------------------------------------------------
// class ForNode
//
//------------------------------------------------------------------------------

    ForNode::~ForNode() {}

    std::string ForNode::str()
    {
      std::string s;
      s += "for";
      s += " ";
      s += this->init_->str() + " ";
      s += this->cond_->str() + " ";
      s +=(this->afterthought_ ? this->afterthought_->str() : "()") + "\n";
      s += this->body_->str();
      return "(" + s + ")";
    }

    ForNode::ForNode(std::vector<Node*> v)
    {
      assert(v.size() <= 4);
      // v[0] : int i;
      // v[1] : i < n;
      // v[2] :
      // v[3] :
      this->init_ = (ExpNode*)v[0];
      this->cond_ = (ExpNode*)v[1];
      this->afterthought_ = (4 == v.size()) ? (ExpNode*)v[2] : NULL;
      this->body_ = (StatNode*)v.back();
    }

//------------------------------------------------------------------------------
// class VarDeclNode
//
//------------------------------------------------------------------------------

    VarNode::~VarNode() {}

    std::string VarNode::str()
    {
      std::string s;
      s += this->name_;
      s += ":";
      s += this->type_;
      return "(" + s + ")";
    }

    VarNode::VarNode(std::vector<Node*> v)
    {
      assert(v.size() <= 2);
      this->type_ = v[0]->str();
      this->name_ = v[1]->str();
    }

//------------------------------------------------------------------------------
// class InitListNode
//
//------------------------------------------------------------------------------

    InitListNode::~InitListNode() {}

    std::string InitListNode::str()
    {
      std::string s;
      for (size_t i = 0; i < this->v_.size(); ++i) {
        s += v_[i]->str() + ", ";
      }
      return "#{" + s + "}";
    }
    bool InitListNode::isAssignable(void) { return false; }
    bool InitListNode::isConst(void) { return true; }
    bool InitListNode::isLvalue(void) { return false; }
    int InitListNode::allocSize(void)
    {
      int sum = 0;
      each (i, v_) {
        sum += (*i)->allocSize();
      }
      return sum;
    }

    InitListNode::InitListNode(std::vector<Node*> v)
    {
      for (size_t i = 0; i < v.size(); ++i) {
        this->v_.push_back((ExpNode*)v[i]);
      }
    }

//------------------------------------------------------------------------------
// class AssignNode
//
//------------------------------------------------------------------------------

    AssignNode::~AssignNode() {}

    std::string AssignNode::str()
    {
      std::string s;
      s += this->left_->str();
      s += " = ";
      s += this->right_->str();
      return "(" + s + ")";
    }
    bool AssignNode::isAssignable(void) { return true; }
    bool AssignNode::isConst(void) { return true; }
    bool AssignNode::isLvalue(void) { return true; }
    int AssignNode::allocSize(void)
    {
      return this->left_->allocSize();
    }

    AssignNode::AssignNode(std::vector<Node*> v)
    {
      this->left_ = (ExpNode*)v[0];
      this->right_ = (ExpNode*)v[1];
    }

  }
}
