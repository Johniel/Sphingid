#ifndef _PARSER_LEXER_HPP_
#define _PARSER_LEXER_HPP_

#include <string>
#include <vector>

#include <boost/xpressive/xpressive.hpp>
using namespace boost::xpressive;

#include "../ast/ast.hpp"

namespace sphingid
{
  namespace parser
  {
    enum TokenType { NumberTokenType,
                     StringTokenType,
                     IdentifierTokenType,
                     SentinelTokenType, } ;

    class Token
    {
    public:
      Token();
      Token(int line);
      virtual int line(void) const;
      virtual bool isIdToken(void) const ;
      virtual bool isNumToken(void) const ;
      virtual bool isStrToken(void) const ;
      virtual std::string str(void) const = 0;
    protected:
      int line_;
      TokenType type_;
    };

    class Number : public Token
    {
    public:
      Number();
      Number(int line, std::string text) ;
      std::string str(void) const;
      virtual bool isNumToken(void) const ;
    private:
      int num_;
    };

    class String : public Token
    {
    public:
      String();
      String(int line, std::string text) ;
      virtual bool isStrToken(void) const ;
      std::string str(void) const;
    private:
      std::string text_;
    };

    class Identifier : public Token
    {
    public:
      Identifier();
      Identifier(int line, std::string text);
      virtual bool isIdToken(void) const ;
      std::string str(void) const;
    protected:
      std::string text_;
    };

    class Sentinel : public Token
    {
    public:
      Sentinel(int line, std::string name);
      virtual ~Sentinel();
      std::string str(void) const;
    protected:
      std::string name_;
    };

    class Lexer
    {
    public:
      Lexer();
      const Token* peek(int i);
      const Token* front(void);
      bool hasNext(void);
      void pop(void);

      static Token* EOL_;
      Token* EOF_;
    private:
      bool readNextLine(void);
      void initRegexp(void);

      sregex reg;
      sregex numlit;
      sregex strlit;
      sregex identifier;
      sregex op;

      int line_count_;
      std::vector<Token*> tokens_;
    };

    int lexer_test(void);
  }
}

#endif /* _PARSER_LEXER_HPP_ */
