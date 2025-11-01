#pragma once

#include "Interpreter.h"

#include "lib/utility/include/Logger.h"

namespace uic918::detail
{
  class VDVInterpreter : public Interpreter
  {
    ::utility::Logger logger;

  public:
    static TypeIdType getTypeId();

    VDVInterpreter(::utility::LoggerFactory &loggerFactory);

    virtual Context interpret(Context &&context) override;
  };
}
