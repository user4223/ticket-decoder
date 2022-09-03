#pragma once

#include "Context.h"
#include "Field.h"

#include <string>
#include <map>
#include <memory>

namespace uic918::detail
{
  struct Interpreter
  {
    virtual ~Interpreter() = default;

    virtual Context &interpret(Context &context) = 0;

    static std::unique_ptr<Context> interpret(Context::BytesType const &input);
  };
}