#pragma once

#include "Interpreter.h"

#include "lib/utility/include/LoggingFwd.h"
#include "lib/utility/include/SignatureChecker.h"

#include <memory>

namespace uic918::detail
{
  class Uic918Interpreter : Interpreter
  {
    ::utility::LoggerFactory &loggerFactory;
    ::utility::SignatureChecker const &signatureChecker;
    ::utility::Logger logger;
    std::unique_ptr<Context> messageContext;

  public:
    Uic918Interpreter(::utility::LoggerFactory &loggerFactory, ::utility::SignatureChecker const &signatureChecker);

    virtual Context &interpret(Context &context) override;
  };
}
