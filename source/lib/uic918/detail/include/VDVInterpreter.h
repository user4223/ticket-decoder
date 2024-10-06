#pragma once

#include "Interpreter.h"

#include "lib/utility/include/Logger.h"

#include <memory>

namespace uic918::detail
{
  class VDVInterpreter : Interpreter
  {
    ::utility::Logger logger;

  public:
    VDVInterpreter(::utility::LoggerFactory &loggerFactory);

    virtual Context interpret(Context &&context) override;
  };
}
