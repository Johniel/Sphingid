#include "interpreter.hpp"

namespace sphingid {
  namespace interpreter {

    Eval::Eval()
    {
      this->env_ = new environment::Environment();
    }

    Eval::Eval(parser::Parser* parser)
    {
    }

    ast::Node* Eval::run(void)
    {
    }

    ast::Node* Eval::run(ast::Node* node)
    {
      node->eval(this);

      vector<ast::Node*> v;
      v.push_back((ast::Node*)(new ast::StrLiteralNode("main")));
      ast::Node* entry = ast::FnCallNode::make(v);
      entry->eval(this);
    }

    ast::Node* Eval::visit(ast::Node* node) {}
    ast::Node* Eval::visit(ast::ArrayNode* node) {}
    ast::Node* Eval::visit(ast::RootNode* node) {}
    ast::Node* Eval::visit(ast::ExpNode* node) {}
    ast::Node* Eval::visit(ast::BinaryOpNode* node) {}
    ast::Node* Eval::visit(ast::NumLiteralNode* node) {}
    ast::Node* Eval::visit(ast::StrLiteralNode* node) {}
    ast::Node* Eval::visit(ast::KeywordNode* node) {}
    ast::Node* Eval::visit(ast::TermSymbolNode* node) {}
    ast::Node* Eval::visit(ast::StatNode* node) {}

    ast::Node* Eval::visit(ast::FnDefNode* node)
    {
      cout << "Eval::visit "  << node->str() << endl;
      this->env_->local(node, env_);
      return NULL;
    }

    ast::Node* Eval::visit(ast::FnDeclNode* node) {}
    ast::Node* Eval::visit(ast::ClassNode* node) {}
    ast::Node* Eval::visit(ast::VarNode* node)
    {
      this->env_->add(node->str(), node);
    }
    ast::Node* Eval::visit(ast::StructNode* node) {}
    ast::Node* Eval::visit(ast::FnCallNode* node) {}
    ast::Node* Eval::visit(ast::SelectionNode* node) {}
    ast::Node* Eval::visit(ast::WhileNode* node) {}
    ast::Node* Eval::visit(ast::JumpNode* node) {}
    ast::Node* Eval::visit(ast::ForNode* node) {}
    ast::Node* Eval::visit(ast::InitListNode* node) {}
    ast::Node* Eval::visit(ast::AssignNode* node) {}

  }
}
