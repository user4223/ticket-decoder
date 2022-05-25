
#include "../include/Interpreter.h"
#include "../include/TLB1Interpreter.h"

std::map<std::string, std::string> Interpreter::create(std::vector<std::uint8_t> const &input)
{
  return std::move(TLB1Interpreter().interpret(Context{input}).output);
}