#pragma once

#include "Context.h"

namespace uic918::detail
{
  struct Interpreter
  {
    virtual ~Interpreter() = default;

    virtual Context interpret(Context &&context) = 0;
  };
}
