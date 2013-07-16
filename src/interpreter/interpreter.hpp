#ifndef _INTERPRETER_INTERPRETER_HPP_
#define _INTERPRETER_INTERPRETER_HPP_

#include "./environment.hpp"
#include "../parser/lexer.hpp"
#include "../parser/parser.hpp"
#include "../ast/ast.hpp"

namespace sphingid {
  namespace interpreter {

    class Eval : public sphingid::ast::Visitor {
    public:
      Eval();
      Eval(sphingid::parser::Parser*);
      ast::Node* run(void);
      ast::Node* run(ast::Node*);
      ast::Node* visit(ast::Node*);
      ast::Node* visit(ast::ArrayNode*);
      ast::Node* visit(ast::RootNode*);
      ast::Node* visit(ast::ExpNode*);
      ast::Node* visit(ast::BinaryOpNode*);
      ast::Node* visit(ast::NumLiteralNode*);
      ast::Node* visit(ast::StrLiteralNode*);
      ast::Node* visit(ast::KeywordNode*);
      ast::Node* visit(ast::TermSymbolNode*);
      ast::Node* visit(ast::StatNode*);
      ast::Node* visit(ast::FnDefNode*);
      ast::Node* visit(ast::FnDeclNode*);
      ast::Node* visit(ast::ClassNode*);
      ast::Node* visit(ast::VarNode*);
      ast::Node* visit(ast::StructNode*);
      ast::Node* visit(ast::FnCallNode*);
      ast::Node* visit(ast::SelectionNode*);
      ast::Node* visit(ast::WhileNode*);
      ast::Node* visit(ast::JumpNode*);
      ast::Node* visit(ast::ForNode*);
      ast::Node* visit(ast::InitListNode*);
      ast::Node* visit(ast::AssignNode*);
      ast::Node* get(std::string name) { return env_->get(name); }
      void addFnScope(ast::Node*, Environment*);
      environment::Environment* getFnScope(ast::Node);
    private:
      ast::Node* program_;
      environment::Environment* env_;
    };

    class Interpreter {
    public:
      Interpreter(parser::Lexer* lexer, parser::Parser* parser);
      void run();
    private:
      environment::Environment* global_;
      parser::Lexer* lexer_;
      parser::Parser* parser_;
    };

  } // interpreter
} // sphingid

#endif /* _INTERPRETER_INTERPRETER_HPP_ */
