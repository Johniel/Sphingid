#include "parser.hpp"

namespace sphingid
{
  namespace parser
  {

//------------------------------------------------------------------------------
// class Rule
//
//------------------------------------------------------------------------------
    class Rule
    {
    public:
      Rule() : next_(NULL) {}

      virtual ~Rule() {}

      virtual int match(Lexer* lexer)
      {
        return match(lexer, 0);
      }

      virtual int match(Lexer*, int) = 0;

      virtual void parse(Lexer*, std::vector<ast::Node*>&) = 0;

    private:
      Rule* next_;
    };

//------------------------------------------------------------------------------
// class NonTerm
//
//------------------------------------------------------------------------------

    class NonTerm : public Rule {
    public:
      NonTerm(Parser* p) : p_(p) {}

      virtual ~NonTerm() {}

      void parse(Lexer* lexer, std::vector<ast::Node*>& result)
      {
        result.push_back(p_->parse(lexer));
      }

     int match(Lexer* lexer, int nth)
      {
        return p_->match(lexer, nth);
      }

      int match(Lexer* lexer)
      {
        return p_->match(lexer);
      }

    private:
      Parser* p_;
    };

//------------------------------------------------------------------------------
// class Or
//
//------------------------------------------------------------------------------

    class Or : public Rule {
    public:
      Or(Parser* p0,
         Parser* p1,
         Parser* p2 = NULL,
         Parser* p3 = NULL,
         Parser* p4 = NULL,
         Parser* p5 = NULL,
         Parser* p6 = NULL,
         Parser* p7 = NULL,
         Parser* p8 = NULL,
         Parser* p9 = NULL)
      {
        if (p0) ps_.push_back(p0);
        if (p1) ps_.push_back(p1);
        if (p2) ps_.push_back(p2);
        if (p3) ps_.push_back(p3);
        if (p4) ps_.push_back(p4);
        if (p5) ps_.push_back(p5);
        if (p6) ps_.push_back(p6);
        if (p7) ps_.push_back(p7);
        if (p8) ps_.push_back(p8);
        if (p9) ps_.push_back(p9);
      }

      virtual ~Or()
      {
        each (i, ps_) delete (*i);
      }

      void parse(Lexer* lexer, std::vector<ast::Node*>& result)
      {
        Parser* p = firstMatch(lexer, 0).first;
        result.push_back(p->parse(lexer));
      }

      int match(Lexer* lexer, int nth)
      {
        return firstMatch(lexer, nth).second;
      }

      std::pair<Parser*, int> firstMatch(Lexer* lexer, int nth)
      {
        each (i, ps_) {
          int m = (*i)->match(lexer, nth);
          if (m) return std::make_pair(*i, m);
        }
        return std::make_pair((Parser*)NULL, 0);
      }

    private:
      std::vector<Parser*> ps_;
    };

//------------------------------------------------------------------------------
// class Repeat
//
//------------------------------------------------------------------------------

    class Repeat : public Rule {
    public:
      Repeat(Parser* p) : p_(p) {}

      virtual ~Repeat() {}

      int match(Lexer* lexer, int nth)
      {
        int sum = nth;
        while (true) {
          int m = p_->match(lexer, sum);
          if (m == 0) break;
          sum += m;
        }
        return sum - nth;
      }

      void parse(Lexer* lexer, std::vector<ast::Node*>& result)
      {
        while (((Rule*)this)->match(lexer)) {
          result.push_back(p_->parse(lexer));
        }
        return ;
      }

    private:
      Parser* p_;
    };

//------------------------------------------------------------------------------
// class Option
//
//------------------------------------------------------------------------------

    class Option : public Rule {
    public:
      Option(Parser* p) : p_(p) {}

      virtual ~Option() {}

      int match(Lexer* lexer)
      {
        return p_->match(lexer);
      }

      void parser(Lexer* lexer, std::vector<ast::Node*>& result)
      {
        result.push_back(p_->parse(lexer));
      }

    private:
      Parser* p_;
    };

//------------------------------------------------------------------------------
// class PrimaryToken
//
//------------------------------------------------------------------------------

    class PrimaryToken : public Rule {
    public:
      PrimaryToken() {}

      virtual ~PrimaryToken() {}

      void parse(Lexer* lexer, std::vector<ast::Node*>& result)
      {
        const Token* t = lexer->front();
        lexer->pop();
        result.push_back(make(t));
        return ;
      }

    protected:
      virtual ast::Node* make(const Token* token) = 0;
    };

//------------------------------------------------------------------------------
// class IdToken
//
//------------------------------------------------------------------------------

    class IdToken : public PrimaryToken {
    public:
      IdToken() {}

      IdToken(std::set<std::string> ignores) : ignores_(ignores) {}

      virtual ~IdToken() {}

      int match(Lexer* lexer, int nth)
      {
        if (ignores_.count(lexer->peek(nth)->str())) return false;
        return lexer->peek(nth)->isIdToken();
      }

      ast::Node* make(const Token* t)
      {
        assert(!ignores_.count(t->str()));
        return new ast::TermSymbolNode(t->str());
      }

    private:
      std::set<std::string> ignores_;
    };

//------------------------------------------------------------------------------
// class NumToken
//
//------------------------------------------------------------------------------

    class NumToken : public PrimaryToken {
    public:
      NumToken() {}
      virtual ~NumToken() {}
      int match(Lexer* lexer, int nth)
      {
        return lexer->peek(nth)->isNumToken();
      }
      ast::Node* make(const Token* t)
      {
        return new ast::NumLiteralNode(atoi(t->str().c_str()));
      }
    };

//------------------------------------------------------------------------------
// class StrToken
//
//------------------------------------------------------------------------------

    class StrToken : public PrimaryToken {
    public:
      StrToken() {}
      virtual ~StrToken() {}
      int match(Lexer* lexer, int nth)
      {
        return lexer->peek(nth)->isStrToken();
      }
      ast::Node* make(const Token* t)
      {
        return new ast::StrLiteralNode(t->str());
      }
    };

//------------------------------------------------------------------------------
// class TermSymbol
//
//------------------------------------------------------------------------------

    class TermSymbol : public Rule {
    public:
      TermSymbol(std::string s) : s_(s) {}
      virtual ~TermSymbol() {}
    protected:
      std::string s_;
    };

//------------------------------------------------------------------------------
// class Cons
//
//------------------------------------------------------------------------------

    class Cons : public TermSymbol {
    public:
      Cons(std::string s) : TermSymbol(s) {}
      virtual ~Cons() {}
      int match(Lexer* lexer, int nth)
      {
        const Token* t = lexer->peek(nth);
        if (t->isIdToken() && t->str() == s_) {
          return 1;
        }
        return 0;
      }
      virtual void parse(Lexer* lexer, std::vector<ast::Node*>& result)
      {
        const Token* t = lexer->front();
        lexer->pop();
        result.push_back(new ast::TermSymbolNode(t->str()));
      }
    };

//------------------------------------------------------------------------------
// class Skip
//
//------------------------------------------------------------------------------

    class Skip : public Cons {
    public:
      Skip(std::string s) : Cons(s) {}
      virtual ~Skip() {}
      void parse(Lexer* lexer, std::vector<ast::Node*>& result)
      {
        assert(lexer->front()->str() == this->s_);
        lexer->pop();
        return ;
      }
    };

//------------------------------------------------------------------------------
// class BinaryOperatorR
//
//------------------------------------------------------------------------------

    class BinaryOpNodeR : public ast::BinaryOpNode {
    public:
      static ast::Node* make(std::vector<ast::Node*> v)
      {
        assert(false);
        return NULL;
      }
    };

//------------------------------------------------------------------------------
// class BinaryOperatorL
//
//------------------------------------------------------------------------------

    class BinaryOpNodeL : public ast::BinaryOpNode {
    public:
      static Node* make(std::vector<Node*> v)
      {
        std::vector<ast::Node*> u;
        std::vector<ast::Node*> w;
        ast::ExpNode* head = (ExpNode*)v[0];

        for (size_t i = 1; i < v.size(); ++i) {
          ast::ArrayNode* curr = (ast::ArrayNode*)v[i];
          u.push_back(curr->nth(0));
          w.push_back(curr->nth(1));
        }

        BinaryOpNode* result = new BinaryOpNode(u[0]->str(), head, (ExpNode*)w[0]);
        const size_t nest = u.size();
        for (size_t i = 1; i < nest; ++i) {
          result = new BinaryOpNode(u[i]->str(), result, (ExpNode*)w[i]);
        }
        return result;
      }
    };

//------------------------------------------------------------------------------
// class Parser
//
//------------------------------------------------------------------------------

    Parser::Parser() {}

    Parser::Parser(MakeNodeFunc f) : makeNodeFunc_(f) {}
    // Parser::Parser(MakeNodeFunc f, std::string name) : makeNodeFunc_(f), name_(name) {}

    Parser::~Parser()
    {
      each (i, rs_) if (*i) delete (*i);
    }

    int Parser::match(Lexer* lexer)
    {
      return match(lexer, 0);
    }

    int Parser::match(Lexer* lexer, int nth)
    {
      if (rs_.empty()) {
        std::cerr << "[" << this->name_ << "] " << "empty parse rule" << std::endl;
        assert(false);
      }

      const Token* front = lexer->peek(nth);
      if (memo_.count((Token*)front)) return memo_[(Token*)front];

      int curr = nth;
      each (i, rs_) {
        int m = (*i)->match(lexer, curr);
        if (m == 0) {
#ifdef TEST
          std::cout << "[" << this->name_ << "] Fail: " << lexer->peek(nth)->str() << std::endl;
#endif
          return memo_[(Token*)front] = 0;
        }
        curr += m;
      }
#ifdef TEST
      std::cout << "[" << this->name_ << "] Succes: " << lexer->peek(nth)->str() << std::endl;
#endif
      return memo_[(Token*)front] = curr - nth;
    }

    ast::Node* Parser::parse(Lexer* lexer)
    {
      std::vector<ast::Node*> result;
      each (i, rs_) (*i)->parse(lexer, result);

#ifdef TEST
      cout << rs_.size() << endl;
      for (int i = 0; i < result.size(); ++i) {
        cout << result[i]->str() << endl;
      }
#endif

      return (makeNodeFunc_)(result);
    }

    Parser* Parser::oneOf(Parser* p0,
                          Parser* p1, Parser* p2, Parser* p3,
                          Parser* p4, Parser* p5, Parser* p6,
                          Parser* p7, Parser* p8, Parser* p9)
    {
      rs_.push_back(new Or(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9));
      return this;
    }

    Parser* Parser::nt(Parser* p)
    {
      rs_.push_back(new NonTerm(p));
      return this;
    }

    Parser* Parser::rep(Parser* p)
    {
      rs_.push_back(new Repeat(p));
      return this;
    }

    Parser* Parser::opt(Parser* p)
    {
      assert(false);
      // rs_.push_back(new Option(p));
      return this;
    }

    Parser* Parser::cons(std::string s)
    {
      rs_.push_back(new Cons(s));
      return this;
    }

    Parser* Parser::skip(std::string s)
    {
      rs_.push_back(new Skip(s));
      return this;
    }

    Parser* Parser::num(void)
    {
      rs_.push_back(new NumToken());
      return this;
    }

    Parser* Parser::str(void)
    {
      rs_.push_back(new StrToken());
      return this;
    }

    // Parser* Parser::id(std::set<std::string> reserved = std::set<std::string>())
    Parser* Parser::id(std::set<std::string> reserved)
    {
      rs_.push_back(new IdToken(reserved));
      return this;
    }

    Parser* Parser::opR(Parser* l, Parser* op, Parser* r)
    {
      Parser* rest = rule<ast::ArrayNode>()->nt(op)->nt(r);
      Parser* parser = rule<BinaryOpNodeR>()->nt(l)->rep(rest);
      this->nt(parser);
      return this;
    }

    Parser* Parser::opL(Parser* l, Parser* op, Parser* r)
    {
      Parser* rest = rule<ast::ArrayNode>()->nt(op)->nt(r);
      Parser* parser = rule<BinaryOpNodeL>()->nt(l)->rep(rest);
      this->nt(parser);
      return this;
    }
  }
}
