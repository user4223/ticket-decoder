
#include "../include/Interpreter.h"

#include "lib/uic918/detail/include/Context.h"
#include "lib/uic918/detail/include/Interpreter.h"

#include "lib/utility/include/Logging.h"

#include <nlohmann/json.hpp>

namespace uic918::api
{

  struct UicInterpreter : public Interpreter
  {
    ::utility::LoggerFactory &loggerFactory;
    ::utility::SignatureChecker const &signatureChecker;

    UicInterpreter(::utility::LoggerFactory &lf, ::utility::SignatureChecker const &sc)
        : loggerFactory(lf), signatureChecker(sc)
    {
    }

    virtual std::optional<std::string> interpret(std::vector<std::uint8_t> const &input, unsigned int indent = 0) const override
    {
      if (input.empty())
      {
        return std::nullopt;
      }
      auto const json = detail::Interpreter::interpret(loggerFactory, input)->getJson();
      if (!json)
      {
        return std::nullopt;
      }
      return nlohmann::json::parse(*json).dump(indent);
    }

    virtual std::map<std::string, Record> interpretRecords(std::vector<std::uint8_t> const &input) const override
    {
      if (input.empty())
      {
        return {};
      }
      return detail::Interpreter::interpret(loggerFactory, input)->getRecords();
    }
  };

  std::unique_ptr<Interpreter>
  Interpreter::create(
      ::utility::LoggerFactory &loggerFactory,
      ::utility::SignatureChecker const &signatureChecker)
  {
    return std::make_unique<UicInterpreter>(loggerFactory, signatureChecker);
  }

}
