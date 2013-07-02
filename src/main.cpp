#include <iostream>
#include <set>
#include <map>
#include <vector>

#include "./parser/parser.hpp"

using namespace std;
using namespace sphingid::parser;
using namespace sphingid::ast;

void arithmetic(void)
{
  Parser* expr = Parser::rule("<EXPR>");
  Parser* fact = Parser::rule("<FACT>");
  Parser* term = Parser::rule("<TERM>");
  Parser* program = Parser::rule<RootNode>("<PROGRAM>");

  expr->oneOf(Parser::rule<BinaryOpNode>("<ADD>")->nonTerm(term)->cons("+")->nonTerm(expr),
              Parser::rule<BinaryOpNode>("<SUB>")->nonTerm(term)->cons("-")->nonTerm(expr),
              Parser::rule("{EXPR->TERM}")->nonTerm(term));

  term->oneOf(Parser::rule<BinaryOpNode>("<MUL>")->nonTerm(fact)->cons("*")->nonTerm(term),
              Parser::rule<BinaryOpNode>("<DIV>")->nonTerm(fact)->cons("/")->nonTerm(term),
              Parser::rule<BinaryOpNode>("<MOD>")->nonTerm(fact)->cons("%")->nonTerm(term),
              Parser::rule("{TERM->FACT}")->nonTerm(fact));

  fact->oneOf(Parser::rule("<BRACKET>")->skip("(")->nonTerm(expr)->skip(")"),
              Parser::rule("<NUM>")->num());

  program->repeat(Parser::rule("<BLOCK>")->nonTerm(expr)->skip(";"));

  Lexer* lexer = new Lexer();
  Node* root = program->parse(lexer);
  cout << root->str() << endl;

  return ;
}

void add_op()
{
  Parser* add = Parser::rule<BinaryOpNode>("<ADD>")->num()->cons("+")->num();

  Lexer* lexer = new Lexer();
  Node* root = add->parse(lexer);

  cout << ">> " << root->str() << endl;

  return ;
}

void sphingid_syntax()
{
  Parser* program = Parser::rule<RootNode>();

  Parser* primary_exp;
  Parser* postfix_exp;
  Parser* arg_list;
  Parser* arg_type;
  Parser* unary_op;
  Parser* unary_exp;
  Parser* multiplicative;
  Parser* additive;
  Parser* shift;
  Parser* relational;
  Parser* equality;
  Parser* bitwise_and;
  Parser* bitwise_or;
  Parser* logical_and;
  Parser* logical_or;
  Parser* exclusive_or;
  Parser* inclusive_or;
  Parser* conditional;
  Parser* assignment_op;
  Parser* assignment_exp;
  Parser* exp;
  Parser* const_exp;

  Parser* keyword;

  Parser* fn_def; // = Parser::rule<FnDefNode>();
  Parser* fn_decl; // = Parser::rule<FnDeclNode>();

  Parser* op_def; // = Parser::rule<OpDefNode>();
  Parser* op_decl; // = Parser::rule<OpDeclNode>();

  Parser* class_def; // = Parser::rule<DefClassNode>();
  Parser* class_body; // = Parser::rule<DefClassNode>();

  Parser* struct_def; // = Parser::rule<StructDefNode>();
  Parser* struct_body;

  Parser* compound_stat;
  Parser* selection_stat;
  Parser* iteration_stat;

  std::set<string> reserved;
  reserved.insert("=");

  unary_op->oneOf(Parser::rule()->cons("+"),
                  Parser::rule()->cons("-"),
                  Parser::rule()->cons("!"),
                  Parser::rule()->cons("~"));
  unary_exp->oneOf(postfix_exp,
                   Parser::rule()->cons("++")->nonTerm(unary_exp),
                   Parser::rule()->cons("--")->nonTerm(unary_exp),
                   Parser::rule()->nonTerm(unary_op)->nonTerm(unary_exp));

  arg_list->repeat(Parser::rule()->id()->id(reserved)->skip(","))->id();
  arg_type->repeat(Parser::rule()->id()->skip(","))->id();

  op_def->id(reserved)->skip("operator")->id()->skip("(")->nonTerm(arg_type)->skip(")")->skip(";");

  fn_def->id(reserved)->id(reserved)->skip("(")->nonTerm(arg_list)->skip(")")->nonTerm(compound_stat);
  fn_decl->id(reserved)->skip("(")->nonTerm(arg_type)->skip(")")->skip(";");

  class_body->repeat(fn_decl);
  class_def->skip("class")->id(reserved)->skip("{")->nonTerm(class_body)->skip("}")->skip(";");

  struct_def->skip("struct")->id(reserved)->skip("{")->nonTerm(struct_body)->skip("}")->skip(";");

  return ;
}

int main(int argc, char *argv[])
{
  arithmetic();
  // add_op();
  return 0;
}
