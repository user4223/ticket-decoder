
#include "../include/Interpreter.h"
#include "../include/TLBInterpreter.h"

Interpreter::Context Interpreter::create(Context &&context)
{
  return TLBInterpreter().interpret(context);
}

std::map<std::string, std::string> Interpreter::create(BytesType const &input)
{
  auto context = Context{input};
  return std::move(create(std::move(context)).output);
}