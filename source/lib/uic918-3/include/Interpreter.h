#pragma once

#include <map>
#include <string>
#include <vector>

class Interpreter
{
public:
  struct Context
  {
    std::vector<std::uint8_t> const &input;
    std::vector<std::uint8_t>::const_iterator position;
    std::map<std::string, std::string> output;

    Context(std::vector<std::uint8_t> const &i) : input(i), position(input.begin()), output() {}
  };

  virtual Context interpret(Context &&context) = 0;

  static std::map<std::string, std::string> create(std::vector<std::uint8_t> const &input);
};