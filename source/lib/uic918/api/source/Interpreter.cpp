
#include "../include/Interpreter.h"

#include "lib/uic918/detail/include/Context.h"
#include "lib/uic918/detail/include/Uic918Interpreter.h"

#include "lib/utility/include/Logging.h"

namespace uic918::api
{

  struct Internal : public Interpreter
  {
    std::unique_ptr<detail::Uic918Interpreter> interpreter;

    Internal(::utility::LoggerFactory &lf, SignatureChecker const &sc)
        : interpreter(std::make_unique<detail::Uic918Interpreter>(lf, sc))
    {
    }

    virtual std::optional<std::string> interpret(std::vector<std::uint8_t> const &input, int indent = -1) const override
    {
      if (input.empty())
      {
        return std::nullopt;
      }
      return interpreter->interpret(detail::Context(input)).getJson(indent);
    }

    virtual std::map<std::string, Record> interpretRecords(std::vector<std::uint8_t> const &input) const override
    {
      if (input.empty())
      {
        return {};
      }
      return interpreter->interpret(detail::Context(input)).getRecords();
    }
  };

  std::unique_ptr<Interpreter>
  Interpreter::create(
      ::utility::LoggerFactory &loggerFactory,
      SignatureChecker const &signatureChecker)
  {
    return std::make_unique<Internal>(loggerFactory, signatureChecker);
  }

}
