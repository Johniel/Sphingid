#include <iostream>

#include "./parser/parser.hpp"

using namespace std;
using namespace sphingid::parser;
using namespace sphingid::ast;

void arithmetic(void)
{
  Parser* expr = Parser::rule();
  Parser* fact = Parser::rule();
  Parser* term = Parser::rule();

  expr->oneOf(Parser::rule()->nonTerm(term),
              Parser::rule<BinaryOpNode>()->nonTerm(term)->cons("+")->nonTerm(term),
              Parser::rule<BinaryOpNode>()->nonTerm(term)->cons("-")->nonTerm(term));

  term->oneOf(Parser::rule()->nonTerm(fact),
              Parser::rule<BinaryOpNode>()->nonTerm(fact)->cons("*")->nonTerm(fact),
              Parser::rule<BinaryOpNode>()->nonTerm(fact)->cons("/")->nonTerm(fact),
              Parser::rule<BinaryOpNode>()->nonTerm(fact)->cons("%")->nonTerm(fact));

  fact->oneOf(Parser::rule()->cons("(")->nonTerm(expr)->cons(")"),
              Parser::rule()->num());

  Lexer* lexer = new Lexer();
  while (lexer->hasNext()) {
    cout << "front: " << lexer->front()->str() << endl;
    Node* root = expr->parse(lexer);
    cout << root->str() << endl;
  }

  return ;
}

int main(int argc, char *argv[])
{
  arithmetic();
  return 0;
}
