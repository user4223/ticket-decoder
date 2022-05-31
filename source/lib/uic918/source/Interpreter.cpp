
#include "../include/Interpreter.h"
#include "../include/TLBInterpreter.h"

std::map<std::string, std::string> Interpreter::create(BytesType const &input)
{
  auto context = Context{input};
  return std::move(TLBInterpreter().interpret(context).output);
}