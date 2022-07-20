#pragma once

#include "Context.h"
#include "Field.h"
#include "Ticket.h"

#include <string>
#include <map>
#include <memory>

struct Interpreter
{
  virtual ~Interpreter() = default;

  virtual Context &interpret(Context &context) = 0;

  static std::unique_ptr<Context> interpret(Context::BytesType const &input);

  static std::unique_ptr<Ticket> interpretTicket(Context::BytesType const &input);

  static std::map<std::string, Field> interpretRaw(Context::BytesType const &input);
};
