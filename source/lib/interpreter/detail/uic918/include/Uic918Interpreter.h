#pragma once

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/utility/include/LoggingFwd.h"

#include "lib/interpreter/api/include/SignatureVerifier.h"

#include <memory>

namespace interpreter::detail::uic
{
  class Uic918Interpreter : public Interpreter
  {
    ::utility::LoggerFactory &loggerFactory;
    ::utility::Logger logger;
    api::SignatureVerifier const *const signatureChecker;
    std::unique_ptr<Context> messageContext;

  public:
    static TypeIdType getTypeId();

    Uic918Interpreter(::utility::LoggerFactory &loggerFactory, api::SignatureVerifier const &signatureChecker);

    Uic918Interpreter(::utility::LoggerFactory &loggerFactory);

    virtual Context interpret(Context &&context) override;
  };
}
