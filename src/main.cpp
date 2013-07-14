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

  fact->oneOf(Parser::rule("<BRACKET>")->skip("(")->nt(expr)->skip(")"),
              Parser::rule("<NUM>")->num());

  program->rep(Parser::rule("<BLOCK>")->nt(expr)->skip(";"));

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
  Parser* primary_exp = Parser::rule("<PRYMARY>");
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

  Parser* keyword = Parser::rule<KeywordNode>("<KEYWORD>");

  Parser* fn_def = Parser::rule<FnDefNode>("<FnDef>");
  Parser* fn_decl = Parser::rule<FnDeclNode>("<FnDecl>");

  Parser* op_def = Parser::rule(); // = Parser::rule<OpDefNode>();
  Parser* op_decl = Parser::rule(); // = Parser::rule<OpDeclNode>();

  Parser* class_def = Parser::rule<ClassNode>("<ClassDef>");

  Parser* struct_def = Parser::rule("<STRUCT>"); // Parser::rule<StructNode>();

  Parser* compound_stat = Parser::rule<RootNode>("<COMPOUND>");
  Parser* exp_stat = Parser::rule();
  Parser* selection_stat = Parser::rule("<SELECTION>");// Parser::rule<SelectionNode>("<SELECTION>");
  Parser* iteration_stat = Parser::rule();

  std::set<string> reserved;
  reserved.insert("=");

  // Parser* fn = Parser::rule<Lambda>()->id()->skip("(")->nt(arg_list)->skip(")");

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
                     Parser::rule()->skip("(")->nt(exp)->skip(")"));

  Parser* fn_call_arg = Parser::rule()->oneOf(Parser::rule<ArrayNode>()->rep(Parser::rule()->nt(exp)->skip(","))->nt(exp),
                                              Parser::rule<ArrayNode>()->nt(exp));

  postfix_exp->oneOf(Parser::rule<FnCallNode>()->id()->skip("(")->skip(")"),
                     Parser::rule<FnCallNode>()->id()->skip("(")->nt(fn_call_arg)->skip(")"),
                     Parser::rule<FnCallNode>()->skip("(")->nt(exp)->skip(")")->skip("(")->nt(fn_call_arg)->skip(")"),
                     Parser::rule()->id()->skip("[")->nt(exp)->skip("]"),
                     Parser::rule()->skip("(")->nt(exp)->skip(")")->skip("[")->nt(exp)->skip("]"),
                     primary_exp);

  unary_exp->oneOf(postfix_exp,
                   Parser::rule()->cons("++")->nt(unary_exp),
                   Parser::rule()->cons("--")->nt(unary_exp),
                   Parser::rule()->cons("+")->nt(unary_exp),
                   Parser::rule()->cons("-")->nt(unary_exp),
                   Parser::rule()->cons("!")->nt(unary_exp),
                   Parser::rule()->cons("~")->nt(unary_exp),
                   Parser::rule()->nt(unary_op)->nt(unary_exp));

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

  conditional->oneOf(Parser::rule()->nt(logical_or)->skip("?")->nt(exp)->skip(":")->nt(conditional),
                     logical_or);

  assignment->oneOf(Parser::rule<AssignNode>()->nt(unary_exp)->skip("=")->nt(assignment),
                    /*
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("*=")->nt(assignment),
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("/=")->nt(assignment),
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("+=")->nt(assignment),
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("-=")->nt(assignment),
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("%=")->nt(assignment),
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("&=")->nt(assignment),
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("|=")->nt(assignment),
                    Parser::rule<AssignNode>()->nt(unary_exp)->cons("^=")->nt(assignment),
                    */
                    conditional);

  exp->nt(assignment);
  exp_stat->nt(exp)->skip(";");

  Parser* jump_stat = Parser::rule("<JUMP>")->oneOf(Parser::rule<JumpNode>("<RETRUN EXP>")->cons("return")->nt(exp_stat),
                                                    Parser::rule<JumpNode>("<RETURN>")->cons("return"),
                                                    Parser::rule<JumpNode>()->cons("break"),
                                                    Parser::rule<JumpNode>()->cons("continue"));

  Parser* var_decl = Parser::rule<VarNode>()->id(reserved)->id(reserved)->skip(";");

  Parser* stat = Parser::rule()->oneOf(compound_stat, selection_stat, iteration_stat, exp_stat, jump_stat, var_decl);

  selection_stat->oneOf(Parser::rule<SelectionNode>()->skip("if")->skip("(")->nt(exp)->skip(")")->nt(stat)->skip("else")->nt(stat),
                        Parser::rule<SelectionNode>()->skip("if")->skip("(")->nt(exp)->skip(")")->nt(stat));

  Parser* for_stat = Parser::rule()->oneOf(Parser::rule<ForNode>()->skip("for")->skip("(")->nt(exp_stat)->nt(exp_stat)->nt(exp)->skip(")")->nt(stat),
                                           Parser::rule<ForNode>()->skip("for")->skip("(")->nt(exp_stat)->nt(exp_stat)->skip(")")->nt(stat));

  Parser* while_stat = Parser::rule<WhileNode>()->skip("while")->skip("(")->nt(exp)->skip(")")->nt(stat);
  Parser* do_stat = Parser::rule()->cons("do");
  iteration_stat->oneOf(for_stat, while_stat, do_stat);

  compound_stat->skip("{")->rep(stat)->skip("}");

  Parser* arg = Parser::rule<ArrayNode>()->id(reserved)->id();
  Parser* arg_list = Parser::rule()->oneOf(Parser::rule<ArrayNode>()->cons("void"),
                                           Parser::rule<ArrayNode>()->rep(Parser::rule()->nt(arg)->skip(","))->nt(arg),
                                           Parser::rule<ArrayNode>()->nt(arg));

  arg_type->oneOf(Parser::rule<ArrayNode>()->rep(Parser::rule()->id()->skip(","))->id(),
                  Parser::rule<ArrayNode>()->id());

  fn_def->id(reserved)->id(reserved)->skip("(")->nt(arg_list)->skip(")")->nt(compound_stat);
  fn_decl->id(reserved)->id(reserved)->skip("(")->nt(arg_type)->skip(")")->skip(";");

  op_def->id(reserved)->skip("operator")->id()->skip("(")->nt(arg_type)->skip(")")->skip(";");

  Parser* class_body = Parser::rule<ArrayNode>()->rep(fn_decl);
  class_def->skip("class")->id(reserved)->skip("{")->nt(class_body)->skip("}")->skip(";");

  Parser* derive = Parser::rule()->oneOf(Parser::rule<ArrayNode>()->skip(":")->rep(Parser::rule()->id()->skip(","))->id(),
                                         Parser::rule<ArrayNode>()->skip(":")->id());

  Parser* struct_body = Parser::rule<ArrayNode>()->rep(var_decl);
  struct_def->oneOf(Parser::rule<StructNode>()->skip("struct")->id(reserved)->skip("{")->nt(struct_body)->skip("}")->skip(";"),
                    Parser::rule<StructNode>()->skip("struct")->id(reserved)->nt(derive)->skip("{")->nt(struct_body)->skip("}")->skip(";"));

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
