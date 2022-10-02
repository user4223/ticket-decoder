
#include "../include/Interpreter.h"

#include "../../detail/include/Context.h"
#include "../../detail/include/Interpreter.h"

#include <nlohmann/json.hpp>

namespace uic918::api
{

  std::optional<std::string> Interpreter::interpret(std::vector<std::uint8_t> const &input)
  {
    return interpret(input, 0);
  }

  std::optional<std::string> Interpreter::interpret(std::vector<std::uint8_t> const &input, unsigned int indent)
  {
    if (input.empty())
    {
      return std::nullopt;
    }
    return detail::Interpreter::interpret(input)->getJson(indent);
  }

  std::map<std::string, Record> Interpreter::interpretRecords(std::vector<std::uint8_t> const &input)
  {
    if (input.empty())
    {
      return {};
    }
    return detail::Interpreter::interpret(input)->getRecords();
  }

}
