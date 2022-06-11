#pragma once

#include <map>
#include <string>
#include <vector>
#include <optional>

class Interpreter
{
public:
  using BytesType = std::vector<std::uint8_t>;

  struct Field
  {
    std::string value;
    std::optional<std::string> description;

    std::string toString() const
    {
      return description ? value + " (" + description.value() + ")" : value;
    }

    static std::tuple<std::string, Field> createEntry(std::string key, std::string value)
    {
      return std::make_tuple(key, Field{value});
    }

    static std::tuple<std::string, Field> createEntry(std::string key, std::string value, std::string description)
    {
      return std::make_tuple(key, Field{value, description});
    }

    static std::tuple<std::string, Field> createEntry(std::string key, std::string value, std::optional<std::string> description)
    {
      return std::make_tuple(key, Field{value, description});
    }

    operator std::string() const
    {
      return toString();
    }
  };

  struct Context
  {
    BytesType const &input;
    std::map<std::string, Field> output;
    std::vector<std::string> recordIds;

    BytesType signature;
    BytesType compressedMessage;
    BytesType uncompressedMessage;
    BytesType::const_iterator position;

    Context(BytesType const &i) : input(i), output() {}
  };

  virtual ~Interpreter() = default;

  virtual Context &interpret(Context &context) = 0;

  static Context interpret(BytesType const &input);
};