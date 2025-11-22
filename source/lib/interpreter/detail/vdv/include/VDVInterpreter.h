#pragma once

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/utility/include/Logger.h"

namespace interpreter::detail::vdv
{
  class VDVInterpreter : public common::Interpreter
  {
    ::utility::Logger logger;

  public:
    static TypeIdType getTypeId();

    VDVInterpreter(::utility::LoggerFactory &loggerFactory);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
