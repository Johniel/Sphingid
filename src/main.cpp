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
  Parser* program = Parser::rule<RootNode>("<PROGRAM>");
  Parser* expr = Parser::rule("<ADD/SUB>");
  Parser* term = Parser::rule("<MUL/DIV/MOD>");
  Parser* fact = Parser::rule("<FACT>");


  Parser* expr_op = Parser::rule()->oneOf(Parser::rule()->cons("+"),
                                          Parser::rule()->cons("-"));

  expr->oneOf(Parser::rule()->operatorL(term, expr_op, term),
              term);

  Parser* term_op = Parser::rule()->oneOf(Parser::rule()->cons("*"),
                                          Parser::rule()->cons("/"),
                                          Parser::rule()->cons("%"));

  term->oneOf(Parser::rule()->operatorL(fact, term_op, fact),
              fact);

  fact->oneOf(Parser::rule("<BRACKET>")->skip("(")->nonTerm(expr)->skip(")"),
              Parser::rule("<NUM>")->num());

  program->repeat(Parser::rule("<BLOCK>")->nonTerm(expr)->skip(";"));

  Lexer* lexer = new Lexer();
  Node* root = program->parse(lexer);
  cout << "$ " << root->str() << endl;

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

  Parser* primary_exp = Parser::rule();
  Parser* postfix_exp = Parser::rule();
  Parser* arg_list = Parser::rule();
  Parser* arg_type = Parser::rule();
  Parser* unary_op = Parser::rule();
  Parser* unary_exp = Parser::rule();
  Parser* multiplicative = Parser::rule();
  Parser* additive = Parser::rule();
  // Parser* shift;
  Parser* relational = Parser::rule();
  Parser* equality = Parser::rule();
  Parser* bitwise_and = Parser::rule();
  Parser* bitwise_or = Parser::rule();
  Parser* logical_and = Parser::rule();
  Parser* logical_or = Parser::rule();
  Parser* exclusive_or = Parser::rule();
  Parser* inclusive_or = Parser::rule();
  Parser* conditional = Parser::rule();
  Parser* assignment = Parser::rule();
  Parser* exp = Parser::rule();
  Parser* const_exp = Parser::rule();

  Parser* keyword = Parser::rule();

  Parser* fn_def = Parser::rule(); // = Parser::rule<FnDefNode>();
  Parser* fn_decl = Parser::rule<FnDeclNode>("<FnDecl>");

  Parser* op_def = Parser::rule(); // = Parser::rule<OpDefNode>();
  Parser* op_decl = Parser::rule(); // = Parser::rule<OpDeclNode>();

  Parser* class_def = Parser::rule<ClassNode>("<ClassDef>");

  Parser* struct_def = Parser::rule(); // = Parser::rule<StructDefNode>();
  Parser* struct_body = Parser::rule();

  Parser* compound_stat = Parser::rule();
  Parser* selection_stat = Parser::rule();
  Parser* iteration_stat = Parser::rule();

  std::set<string> reserved;
  reserved.insert("=");

  unary_exp->oneOf(postfix_exp,
                   Parser::rule()->cons("++")->nonTerm(unary_exp),
                   Parser::rule()->cons("--")->nonTerm(unary_exp),
                   Parser::rule()->cons("+")->nonTerm(unary_exp),
                   Parser::rule()->cons("-")->nonTerm(unary_exp),
                   Parser::rule()->cons("!")->nonTerm(unary_exp),
                   Parser::rule()->cons("~")->nonTerm(unary_exp),
                   Parser::rule()->nonTerm(unary_op)->nonTerm(unary_exp));

  Parser* multiplicative_op = Parser::rule()->oneOf(Parser::rule()->cons("*"),
                                                    Parser::rule()->cons("/"),
                                                    Parser::rule()->cons("%"));

  multiplicative->oneOf(Parser::rule()->operatorL(unary_exp, multiplicative_op, unary_exp),
                        unary_exp);

  Parser* additive_op = Parser::rule()->oneOf(Parser::rule()->cons("+"),
                                              Parser::rule()->cons("-"));
  additive->oneOf(Parser::rule()->operatorL(multiplicative, additive_op, multiplicative),
                  multiplicative);

  Parser* relational_op = Parser::rule()->oneOf(Parser::rule()->cons("<"),
                                                Parser::rule()->cons(">"),
                                                Parser::rule()->cons("<="),
                                                Parser::rule()->cons(">="));

  relational->oneOf(Parser::rule()->operatorL(additive, relational_op, additive),
                    additive);

  Parser* equality_op = Parser::rule()->oneOf(Parser::rule()->cons("=="),
                                              Parser::rule()->cons("!="));

  equality->oneOf(Parser::rule()->operatorL(relational, equality_op, relational),
                  relational);

  bitwise_and->oneOf(Parser::rule()->operatorL(equality, Parser::rule()->cons("&"), equality),
                     equality);

  exclusive_or->oneOf(Parser::rule()->operatorL(bitwise_and, Parser::rule()->cons("^"), bitwise_and),
                      bitwise_and);

  inclusive_or->oneOf(Parser::rule()->operatorL(exclusive_or, Parser::rule()->cons("|"), exclusive_or),
                      exclusive_or);

  logical_and->oneOf(Parser::rule()->operatorL(inclusive_or, Parser::rule()->cons("&&"), inclusive_or),
                     inclusive_or);

  logical_or->oneOf(Parser::rule()->operatorL(logical_and, Parser::rule()->cons("||"), logical_and),
                    logical_and);

  conditional->oneOf(Parser::rule()->nonTerm(logical_or)->skip("?")->nonTerm(exp)->skip(":")->nonTerm(conditional),
                     logical_or);

  assignment->oneOf(conditional,
                    Parser::rule()->nonTerm(unary_exp)->cons("=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("*=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("/=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("+=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("-=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("%=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("&=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("|=")->nonTerm(assignment),
                    Parser::rule()->nonTerm(unary_exp)->cons("^=")->nonTerm(assignment));

  arg_list->repeat(Parser::rule()->id()->id(reserved)->skip(","))->id();
  arg_type->oneOf(Parser::rule<ArrayNode>()->repeat(Parser::rule()->id()->skip(","))->id(),
                  Parser::rule<ArrayNode>()->id());

  op_def->id(reserved)->skip("operator")->id()->skip("(")->nonTerm(arg_type)->skip(")")->skip(";");

  fn_def->id(reserved)->id(reserved)->skip("(")->nonTerm(arg_list)->skip(")")->nonTerm(compound_stat);
  fn_decl->id(reserved)->id(reserved)->skip("(")->nonTerm(arg_type)->skip(")")->skip(";");

  Parser* class_body = Parser::rule<ArrayNode>()->repeat(fn_decl);
  class_def->skip("class")->id(reserved)->skip("{")->nonTerm(class_body)->skip("}")->skip(";");

  struct_def->skip("struct")->id(reserved)->skip("{")->nonTerm(struct_body)->skip("}")->skip(";");

  Lexer* lexer = new Lexer();
  Node* root = class_def->parse(lexer);
  cout << "$" << endl;
  if (root) cout << root->str() << endl;
  return ;
}

int main(int argc, char *argv[])
{
  // arithmetic();
  // add_op();
  sphingid_syntax();
  return 0;
}
