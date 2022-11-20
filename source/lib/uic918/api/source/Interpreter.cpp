
#include "../include/Interpreter.h"

#include "lib/uic918/detail/include/Context.h"
#include "lib/uic918/detail/include/Uic918Interpreter.h"

#include "lib/utility/include/Logging.h"

namespace uic918::api
{

  struct Internal : public Interpreter
  {
    std::unique_ptr<detail::Uic918Interpreter> interpreter;

    Internal(::utility::LoggerFactory &lf, ::utility::SignatureChecker const &sc)
        : interpreter(std::make_unique<detail::Uic918Interpreter>(lf, sc))
    {
    }

    virtual std::optional<std::string> interpret(std::vector<std::uint8_t> const &input, int indent = -1) const override
    {
      if (input.empty())
      {
        return std::nullopt;
      }
      auto context = std::make_unique<detail::Context>(input);
      return interpreter->interpret(*context).getJson(indent);
    }

    virtual std::map<std::string, Record> interpretRecords(std::vector<std::uint8_t> const &input) const override
    {
      if (input.empty())
      {
        return {};
      }
      auto context = std::make_unique<detail::Context>(input);
      return interpreter->interpret(*context).getRecords();
    }
  };

  std::unique_ptr<Interpreter>
  Interpreter::create(
      ::utility::LoggerFactory &loggerFactory,
      ::utility::SignatureChecker const &signatureChecker)
  {
    return std::make_unique<Internal>(loggerFactory, signatureChecker);
  }

}
