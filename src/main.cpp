#include <iostream>

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

  expr->oneOf(Parser::rule<BinaryOpNode>("<ADD>")->nonTerm(term)->cons("+")->nonTerm(term),
              Parser::rule<BinaryOpNode>("<SUB>")->nonTerm(term)->cons("-")->nonTerm(term),
              Parser::rule("{EXPR->TERM}")->nonTerm(term));

  term->oneOf(Parser::rule<BinaryOpNode>("<MUL>")->nonTerm(fact)->cons("*")->nonTerm(fact),
              Parser::rule<BinaryOpNode>("<DIV>")->nonTerm(fact)->cons("/")->nonTerm(fact),
              Parser::rule<BinaryOpNode>("<MOD>")->nonTerm(fact)->cons("%")->nonTerm(fact),
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
  Parser* num = Parser::rule()->num();
  Parser* add = Parser::rule<BinaryOpNode>("<ADD>")->nonTerm(num)->cons("+")->nonTerm(num);
  Lexer* lexer = new Lexer();
  Node* root = add->parse(lexer);
  cout << ">> " << root->str() << endl;
  return ;
}

int main(int argc, char *argv[])
{
  arithmetic();
  // add_op();
  return 0;
}
