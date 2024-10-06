#pragma once

#include "Context.h"

namespace uic918::detail
{
  struct Interpreter
  {
    virtual ~Interpreter() = default;

    using TypeIdType = std::vector<std::uint8_t>;

    virtual Context interpret(Context &&context) = 0;
  };
}
