#pragma once

#include "Interpreter.h"

#include "lib/utility/include/LoggingFwd.h"

#include "lib/uic918/api/include/SignatureChecker.h"

#include <memory>

namespace uic918::detail
{
  class Uic918Interpreter : Interpreter
  {
    ::utility::LoggerFactory &loggerFactory;
    ::utility::Logger logger;
    api::SignatureChecker const *const signatureChecker;
    std::unique_ptr<Context> messageContext;

  public:
    static TypeIdType getTypeId();

    Uic918Interpreter(::utility::LoggerFactory &loggerFactory, api::SignatureChecker const &signatureChecker);

    Uic918Interpreter(::utility::LoggerFactory &loggerFactory);

    virtual Context interpret(Context &&context) override;
  };
}
