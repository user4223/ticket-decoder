
#include "../include/Interpreter.h"
#include "../include/TLBInterpreter.h"

std::map<std::string, std::string> Interpreter::create(BytesType const &input)
{
  return std::move(TLBInterpreter().interpret(Context{input}).output);
}