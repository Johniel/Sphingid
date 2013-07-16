#ifndef _INENVIRONMENT_ENVIRONMENT_H_
#define _INENVIRONMENT_ENVIRONMENT_H_

#include "../ast/ast.hpp"
#include <string>
#include <map>

namespace sphingid {
  namespace environment {

    class Environment {
    public:
      Environment();
      virtual ~Environment();
      void add(std::string, sphingid::ast::Node*);
      void del(std::string);
      ast::Node* get(std::string);

      Environment* nest();
    private:
      std::map<std::string, ast::Node*> context_;
      std::map<ast::string, environment::Environment*> scope_;
      Environment* prev_;
    };

  }
}

#endif /* _INENVIRONMENT_ENVIRONMENT_H_ */
