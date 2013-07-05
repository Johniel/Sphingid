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
              Parser::rule()->nonTerm(term));

  Parser* term_op = Parser::rule()->oneOf(Parser::rule()->cons("*"),
                                          Parser::rule()->cons("/"),
                                          Parser::rule()->cons("%"));

  term->oneOf(Parser::rule()->operatorL(fact, term_op, fact),
              Parser::rule()->nonTerm(fact));

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

  Parser* primary_exp;
  Parser* postfix_exp;
  Parser* arg_list;
  Parser* arg_type;
  Parser* unary_op;
  Parser* unary_exp;
  Parser* multiplicative;
  Parser* additive;
  // Parser* shift;
  Parser* relational;
  Parser* equality;
  Parser* bitwise_and;
  Parser* bitwise_or;
  Parser* logical_and;
  Parser* logical_or;
  Parser* exclusive_or;
  Parser* inclusive_or;
  Parser* conditional;
  Parser* assignment;
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

  unary_exp->oneOf(postfix_exp,
                   Parser::rule()->cons("++")->nonTerm(unary_exp),
                   Parser::rule()->cons("--")->nonTerm(unary_exp),
                   Parser::rule()->cons("+")->nonTerm(unary_exp),
                   Parser::rule()->cons("-")->nonTerm(unary_exp),
                   Parser::rule()->cons("!")->nonTerm(unary_exp),
                   Parser::rule()->cons("~")->nonTerm(unary_exp),
                   Parser::rule()->nonTerm(unary_op)->nonTerm(unary_exp));

  multiplicative->oneOf(unary_exp,
                        Parser::rule()->nonTerm(multiplicative)->cons("*")->nonTerm(unary_exp),
                        Parser::rule()->nonTerm(multiplicative)->cons("/")->nonTerm(unary_exp),
                        Parser::rule()->nonTerm(multiplicative)->cons("%")->nonTerm(unary_exp));

  additive->oneOf(multiplicative,
                  Parser::rule()->nonTerm(additive)->cons("+")->nonTerm(multiplicative),
                  Parser::rule()->nonTerm(additive)->cons("-")->nonTerm(multiplicative));

  relational->oneOf(additive,
                    Parser::rule()->nonTerm(relational)->cons("<")->nonTerm(additive),
                    Parser::rule()->nonTerm(relational)->cons(">")->nonTerm(additive),
                    Parser::rule()->nonTerm(relational)->cons("<=")->nonTerm(additive),
                    Parser::rule()->nonTerm(relational)->cons(">=")->nonTerm(additive));

  equality->oneOf(relational,
                  Parser::rule()->nonTerm(equality)->cons("==")->nonTerm(relational),
                  Parser::rule()->nonTerm(equality)->cons("!=")->nonTerm(relational));

  bitwise_and->oneOf(equality,
                     Parser::rule()->nonTerm(bitwise_and)->cons("&")->nonTerm(equality));

  exclusive_or->oneOf(bitwise_and,
                      Parser::rule()->nonTerm(exclusive_or)->cons("^")->nonTerm(bitwise_and));

  inclusive_or->oneOf(exclusive_or,
                      Parser::rule()->nonTerm(inclusive_or)->cons("|")->nonTerm(exclusive_or));

  logical_and->oneOf(inclusive_or,
                     Parser::rule()->nonTerm(logical_and)->cons("&&")->nonTerm(inclusive_or));

  logical_or->oneOf(logical_and,
                    Parser::rule()->nonTerm(logical_or)->cons("||")->nonTerm(logical_and));

  conditional->oneOf(logical_or,
                     Parser::rule()->nonTerm(logical_or)->skip("?")->nonTerm(exp)->skip(":")->nonTerm(conditional));

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
