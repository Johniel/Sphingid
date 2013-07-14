#ifndef _INTERPRETER_INTERPRETER_HPP_
#define _INTERPRETER_INTERPRETER_HPP_

#include "environment.hpp"

namespace sphingid {
  namespace interpreter {

    class Interpreter {
    public:
      Interpreter();
    private:
      environment::Environment* global_;
    };

  } // interpreter
} // sphingid

#endif /* _INTERPRETER_INTERPRETER_HPP_ */
