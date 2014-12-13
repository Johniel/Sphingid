#ifndef _PERSER_PARSER_HPP_
#define _PERSER_PARSER_HPP_

#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <map>
#include <set>

#include "../ast/ast.hpp"
#include "../macro.hpp"
#include "./lexer.hpp"

namespace {

};

namespace sphingid
{
  namespace parser
  {
    typedef ast::Node* (*MakeNodeFunc)(std::vector<ast::Node*>);

    class Rule;

    class Parser
    {
    public:
      Parser();
      virtual ~Parser();

      template<class T = ast::Node>
      static Parser* rule(std::string name = "{anonymous}")
      {
        Parser* parser = new Parser(T::make);
        parser->name_ = name;
        return parser;
      }

      ast::Node* parse(Lexer*);
      int match(Lexer*);
      int match(Lexer*, int);

      Parser* oneOf(Parser* p0,
                    Parser* p1 = NULL, Parser* p2 = NULL, Parser* p3 = NULL,
                    Parser* p4 = NULL, Parser* p5 = NULL, Parser* p6 = NULL,
                    Parser* p7 = NULL, Parser* p8 = NULL, Parser* p9 = NULL);
      Parser* cons(std::string);
      Parser* skip(std::string);
      Parser* option(Parser*);
      Parser* rep(Parser*);
      Parser* opt(Parser*);
      Parser* nt(Parser*);
      Parser* opR(Parser*, Parser*, Parser*);
      Parser* opL(Parser*, Parser*, Parser*);
      // Parser* id(std::set<std::string>);
      Parser* id(std::set<std::string> reserved = std::set<std::string>());
      Parser* num(void);
      Parser* str(void);
      std::string getName(void) { return name_; }
    private:
      Parser(MakeNodeFunc);
      // Parser(MakeNodeFunc, std::string);
      MakeNodeFunc makeNodeFunc_;
      std::string name_;
      std::vector<Rule*> rs_;
      std::map<Token*, int> memo_;
    };
  }
}

#endif /* _PERSER_PARSER_HPP_ */
