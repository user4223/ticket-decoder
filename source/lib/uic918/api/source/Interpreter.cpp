
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
    auto const json = detail::Interpreter::interpret(input)->getJson();
    if (!json)
    {
      return std::nullopt;
    }
    return nlohmann::json::parse(*json).dump(indent);
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
