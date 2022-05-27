#pragma once

#include <map>
#include <string>
#include <vector>

class Interpreter
{
public:
  using BytesType = std::vector<std::uint8_t>;

  struct Context
  {
    BytesType const &input;
    std::map<std::string, std::string> output;

    BytesType signature;
    BytesType compressedMessage;
    BytesType uncompressedMessage;

    Context(BytesType const &i) : input(i), output() {}
  };

  virtual ~Interpreter() = default;

  virtual Context interpret(Context &&context) = 0;

  static std::map<std::string, std::string> create(BytesType const &input);
};