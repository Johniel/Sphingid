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
          if (EOF_ == (Token*)NULL) EOF_ = new Sentinel(line_count_, "<EOF>");
          return EOF_;
        }
      }
      // std::cout << "peek: " << tokens_[i]->str() << std::endl;
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
      const std::string numlit_regex = "[[:digit:]]+";
      const std::string strlit_regex = "\"(\\\n|\\\"|\\\'|.)*\"";
      const std::string id_regex = "[a-zA-Z_][_a-zA-Z0-9_]*";
      const std::string tokenize_regex = strlit_regex + "|" + numlit_regex + "|" + id_regex + "|" + "[[:punct:]]";

      numlit_     = std::regex(numlit_regex);
      strlit_     = std::regex(strlit_regex);
      identifier_ = std::regex(id_regex);
      tokenize_   = std::regex(tokenize_regex);

      return ;
    }

    bool Lexer::readNextLine(void)
    {
      if (std::cin.eof()) return false;

      std::string line;
      std::getline(std::cin, line);

      std::sregex_iterator first(line.begin(), line.end(), tokenize_), end;

      for (std::sregex_iterator itr = first; itr != end; ++itr) {
        std::string s = itr->str();
        if (std::regex_match(s, numlit_)) {
          tokens_.push_back(new Number(line_count_, s));
        } else if (std::regex_match(s, strlit_)) {
          tokens_.push_back(new String(line_count_, s));
        } else if (std::regex_match(s, identifier_)) {
          tokens_.push_back(new Identifier(line_count_, s));
        } else {
          tokens_.push_back(new Identifier(line_count_, s));
        }
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
        if (token == lexer.EOF_) break;
        lexer.pop();
        std::cout << token->str() << ' ' << std::flush;
      }
      return 0;
    }
  }
}
