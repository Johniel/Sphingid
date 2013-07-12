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

  expr->oneOf(Parser::rule()->opL(term, expr_op, term),
              term);

  Parser* term_op = Parser::rule()->oneOf(Parser::rule()->cons("*"),
                                          Parser::rule()->cons("/"),
                                          Parser::rule()->cons("%"));

  term->oneOf(Parser::rule()->opL(fact, term_op, fact),
              fact);

  fact->oneOf(Parser::rule("<BRACKET>")->skip("(")->nonTerm(expr)->skip(")"),
              Parser::rule("<NUM>")->num());

  program->rep(Parser::rule("<BLOCK>")->nonTerm(expr)->skip(";"));

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
  Parser* primary_exp = Parser::rule();
  Parser* postfix_exp = Parser::rule("<POSTFIX>");
  // Parser* arg_list = Parser::rule();
  Parser* arg_type = Parser::rule();
  Parser* unary_op = Parser::rule();
  Parser* unary_exp = Parser::rule();
  Parser* multiplicative = Parser::rule("<MUL>");
  Parser* additive = Parser::rule("<ADD>");
  // Parser* shift;
  Parser* relational = Parser::rule("<REL>");
  Parser* equality = Parser::rule("<EQ>");
  Parser* bitwise_and = Parser::rule("<&&>");
  Parser* bitwise_or = Parser::rule("<||>");
  Parser* logical_and = Parser::rule("<>");
  Parser* logical_or = Parser::rule("<|>");
  Parser* exclusive_or = Parser::rule("<^>");
  Parser* inclusive_or = Parser::rule("<|>");
  Parser* conditional = Parser::rule("<COND>");
  Parser* assignment = Parser::rule("<ASSIGN>");
  Parser* const_exp = Parser::rule("<CONST EXP>");
  Parser* exp = Parser::rule("<EXP>");

  Parser* keyword = Parser::rule<KeywordNode>();

  Parser* fn_def = Parser::rule<FnDefNode>("<FnDef>");
  Parser* fn_decl = Parser::rule<FnDeclNode>("<FnDecl>");

  Parser* op_def = Parser::rule(); // = Parser::rule<OpDefNode>();
  Parser* op_decl = Parser::rule(); // = Parser::rule<OpDeclNode>();

  Parser* class_def = Parser::rule<ClassNode>("<ClassDef>");

  Parser* struct_def = Parser::rule<StructNode>();

  Parser* compound_stat = Parser::rule<RootNode>();
  Parser* exp_stat = Parser::rule();
  Parser* selection_stat = Parser::rule();
  Parser* iteration_stat = Parser::rule();

  std::set<string> reserved;
  reserved.insert("=");

  // Parser* fn = Parser::rule<Lambda>()->id()->skip("(")->nonTerm(arg_list)->skip(")");

  Parser* type = Parser::rule()->oneOf(Parser::rule()->cons("void"),
                                       Parser::rule()->cons("double"),
                                       Parser::rule()->cons("int"),
                                       Parser::rule()->cons("keyword"),
                                       Parser::rule()->cons("string"));

  keyword->skip(":")->id(reserved);

  primary_exp->oneOf(keyword,
                     Parser::rule()->id(),
                     Parser::rule()->num(),
                     Parser::rule()->str(),
                     Parser::rule()->skip("(")->nonTerm(exp)->skip(")"));

  Parser* fn_call_arg = Parser::rule()->oneOf(Parser::rule<ArrayNode>()->rep(Parser::rule()->nonTerm(exp)->skip(","))->nonTerm(exp),
                                              Parser::rule<ArrayNode>()->nonTerm(exp));

  postfix_exp->oneOf(Parser::rule<FnCallNode>()->id()->skip("(")->skip(")"),
                     Parser::rule<FnCallNode>()->id()->skip("(")->nonTerm(fn_call_arg)->skip(")"),
                     Parser::rule<FnCallNode>()->skip("(")->nonTerm(exp)->skip(")")->skip("(")->nonTerm(fn_call_arg)->skip(")"),
                     Parser::rule()->id()->skip("[")->nonTerm(exp)->skip("]"),
                     Parser::rule()->skip("(")->nonTerm(exp)->skip(")")->skip("[")->nonTerm(exp)->skip("]"),
                     primary_exp);

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

  multiplicative->oneOf(Parser::rule()->opL(unary_exp, multiplicative_op, unary_exp),
                        unary_exp);

  Parser* additive_op = Parser::rule()->oneOf(Parser::rule()->cons("+"),
                                              Parser::rule()->cons("-"));
  additive->oneOf(Parser::rule()->opL(multiplicative, additive_op, multiplicative),
                  multiplicative);

  Parser* relational_op = Parser::rule()->oneOf(Parser::rule()->cons("<"),
                                                Parser::rule()->cons(">"),
                                                Parser::rule()->cons("<="),
                                                Parser::rule()->cons(">="));

  relational->oneOf(Parser::rule()->opL(additive, relational_op, additive),
                    additive);

  Parser* equality_op = Parser::rule()->oneOf(Parser::rule()->cons("=="),
                                              Parser::rule()->cons("!="));

  equality->oneOf(Parser::rule()->opL(relational, equality_op, relational),
                  relational);

  bitwise_and->oneOf(Parser::rule()->opL(equality, Parser::rule()->cons("&"), equality),
                     equality);

  exclusive_or->oneOf(Parser::rule()->opL(bitwise_and, Parser::rule()->cons("^"), bitwise_and),
                      bitwise_and);

  inclusive_or->oneOf(Parser::rule()->opL(exclusive_or, Parser::rule()->cons("|"), exclusive_or),
                      exclusive_or);

  logical_and->oneOf(Parser::rule()->opL(inclusive_or, Parser::rule()->cons("&&"), inclusive_or),
                     inclusive_or);

  logical_or->oneOf(Parser::rule()->opL(logical_and, Parser::rule()->cons("||"), logical_and),
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

  exp->nonTerm(assignment);
  exp_stat->nonTerm(exp)->skip(";");
  compound_stat->skip("{")->rep(exp_stat)->skip("}");

  Parser* arg = Parser::rule<ArrayNode>()->id(reserved)->id();
  Parser* arg_list = Parser::rule()->oneOf(Parser::rule<ArrayNode>()->cons("void"),
                                           Parser::rule<ArrayNode>()->rep(Parser::rule()->nonTerm(arg)->skip(","))->nonTerm(arg),
                                           Parser::rule<ArrayNode>()->nonTerm(arg));

  arg_type->oneOf(Parser::rule<ArrayNode>()->rep(Parser::rule()->id()->skip(","))->id(),
                  Parser::rule<ArrayNode>()->id());

  fn_def->id(reserved)->id(reserved)->skip("(")->nonTerm(arg_list)->skip(")")->nonTerm(compound_stat);
  fn_decl->id(reserved)->id(reserved)->skip("(")->nonTerm(arg_type)->skip(")")->skip(";");

  op_def->id(reserved)->skip("operator")->id()->skip("(")->nonTerm(arg_type)->skip(")")->skip(";");

  Parser* class_body = Parser::rule<ArrayNode>()->rep(fn_decl);
  class_def->skip("class")->id(reserved)->skip("{")->nonTerm(class_body)->skip("}")->skip(";");

  Parser* var_decl = Parser::rule<ArrayNode>()->id(reserved)->id(reserved)->skip(";");
  Parser* struct_body = Parser::rule<ArrayNode>()->rep(var_decl);
  struct_def->skip("struct")->id(reserved)->skip("{")->nonTerm(struct_body)->skip("}")->skip(";");

  Parser* program = Parser::rule<RootNode>()->rep(Parser::rule()->oneOf(class_def, struct_def, fn_def));

  Lexer* lexer = new Lexer();
  Node* root = program->parse(lexer);
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
