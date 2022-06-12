#pragma once

#include "Interpreter.h"

#include <string>

class EFSSegment : Interpreter
{
  std::string prefix;

public:
  EFSSegment(std::string prefix);

  virtual Context &interpret(Context &context) override;
};
