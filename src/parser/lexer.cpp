#include "./lexer.hpp"

namespace sphingid
{
  namespace parser
  {

//------------------------------------------------------------------------------
// class Token
//
//------------------------------------------------------------------------------

    Token::Token() {}

    Token::Token(int line) : line_(line) {}

    int Token::line(void) const
    {
      return line_;
    }

    bool Token::isIdToken(void)  const { return false; }
    bool Token::isNumToken(void) const { return false; }
    bool Token::isStrToken(void) const { return false; }

//------------------------------------------------------------------------------
// class Number
//
//------------------------------------------------------------------------------

    Number::Number() {}

    Number::Number(int line, std::string text)
      : Token(line), num_(atoi(text.c_str()))
    {
      type_ = NumberTokenType;
    }

    std::string Number::str(void) const
    {
      char buff[100];
      sprintf(buff, "%d", num_);
      return std::string(buff);
    }

    bool Number::isNumToken(void) const
    {
      return true;
    }

//------------------------------------------------------------------------------
// class String
//
//------------------------------------------------------------------------------

    String::String() {}

    String::String(int line, std::string text)
      : Token(line), text_(text)
    {
      type_ = StringTokenType;
    }

    std::string String::str(void) const
    {
      return text_;
    }

    bool String::isStrToken(void) const
    {
      return true;
    }

//------------------------------------------------------------------------------
// class Identifier
//
//------------------------------------------------------------------------------

    Identifier::Identifier() {}

    Identifier::Identifier(int line, std::string text)
      : Token(line), text_(text)
    {
      type_ = IdentifierTokenType;
    }

    std::string Identifier::str(void) const
    {
      return text_;
    }

    bool Identifier::isIdToken(void) const
    {
      return true;
    }

//------------------------------------------------------------------------------
// class Sentinel
//  - End of Something
//------------------------------------------------------------------------------

    Sentinel::Sentinel(int line, std::string name)
      : Token(line), name_(name)
    {
      type_ = SentinelTokenType;
    }

    Sentinel::~Sentinel() {}

    std::string Sentinel::str(void) const
    {
      return name_;
    }

//------------------------------------------------------------------------------
// class Lexer
//
//------------------------------------------------------------------------------

    Token* Lexer::EOL_ = new Identifier(-1, "<EOL>\n");

    Lexer::Lexer()
    {
      line_count_ = 1;
      EOF_ = NULL;
      initRegexp();
      return ;
    }

    const Token* Lexer::peek(int i)
    {
      while ((int)tokens_.size() <= i) {
        if (!readNextLine()) {
          if (EOF_ == (Token*)NULL) EOF_ = new Sentinel(line_count_, "<EOF>\n");
          return EOF_;
        }
      }
      return tokens_[i];
    }

    const Token* Lexer::front(void)
    {
      return peek(0);
    }

    void Lexer::pop(void)
    {
      if (tokens_.size()) tokens_.erase(tokens_.begin());
      return ;
    }

    bool Lexer::hasNext(void)
    {
      return front() != EOF_;
    }

    void Lexer::initRegexp(void)
    {
      identifier =
        (as_xpr("_") | range('a', 'z') | range('A', 'Z')) >>
        *(as_xpr("_") | range('a', 'z') | range('A', 'Z') | range('0', '9'));

      op = as_xpr("==") | "<=" | ">=" | "&&" | "||";

      strlit = '\"' >> *(as_xpr("\\\"") | "\\n" | "\\\\" |
                         ~(boost::xpressive::set= '\"', '\'')) >> '\"';

      numlit = *range('0', '9');

      mark_tag identifier_tag(1), op_tag(2), str_tag(3), num_tag(4), punc_tag(5);
      reg =
        *_s >>
        !(("//" >> *_) |
          (identifier_tag= by_ref(identifier)) |
          (op_tag= by_ref(op)) |
          (str_tag= by_ref(strlit)) |
          (num_tag= by_ref(numlit)) |
          (punc_tag= punct));

      return ;
    }

    bool Lexer::readNextLine(void)
    {
      if (std::cin.eof()) return false;

      std::string str;
      std::getline(std::cin, str);

      mark_tag identifier_tag(1), op_tag(2), str_tag(3), num_tag(4), punc_tag(5);

      sregex_iterator itr(str.begin(), str.end(), reg);
      sregex_iterator end;
      for(std::string s; itr != end; ++itr) {

        s = (*itr)[identifier_tag].str();
        if (s.size()) tokens_.push_back(new Identifier(line_count_, s));

        s = (*itr)[op_tag].str();
        if (s.size()) tokens_.push_back(new Identifier(line_count_, s));

        s = (*itr)[str_tag].str();
        if (s.size()) tokens_.push_back(new String(line_count_, s));

        s = (*itr)[num_tag].str();
        if (s.size()) tokens_.push_back(new Number(line_count_, s));

        s = (*itr)[punc_tag].str();
        if (s.size()) tokens_.push_back(new Identifier(line_count_, s));

      }

      ++line_count_;
      return true;
    }

//------------------------------------------------------------------------------

    int lexer_test(void)
    {
      Lexer lexer;
      Token* token;
      while (true) {
        token = (Token*)lexer.front();
        if (token == NULL) break;
        lexer.pop();
        std::cout << token->str() << std::endl;
      }
      return 0;
    }
  }
}
