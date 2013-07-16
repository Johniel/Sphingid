#include "environment.hpp"

namespace sphingid {
  namespace environment {

    Environment::Environment() {}
    Environment::~Environment() {}

    void Environment::add(std::string name, ast::Node* node)
    {
      this->context_[name] = node;
    }

    void Environment::del(std::string name)
    {
      this->context_.erase(name);
      return ;
    }

    ast::Node* Environment::get(std::string name)
    {
      return this->context_[name];
    }

    Environment* Environment::nest() {}

    environment::Environment Environment::getFnScope(ast::Node node)
    {
      return this->scope_[node->str()];
    }

    void Environment::addFnScope(ast::Node* node, Environment* env)
    {
      this->scope_[node->str()] = env;
      return ;
    }
  }
}
