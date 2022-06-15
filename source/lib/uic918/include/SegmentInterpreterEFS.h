#pragma once

#include "Interpreter.h"

#include <string>

class SegmentInterpreterEFS : Interpreter
{
  std::string prefix;

public:
  SegmentInterpreterEFS(std::string prefix);

  virtual Context &interpret(Context &context) override;
};
