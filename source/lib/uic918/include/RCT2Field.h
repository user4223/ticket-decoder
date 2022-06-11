#pragma once

#include <string>

#include "Interpreter.h"

struct RCT2Field
{
  unsigned int line;
  unsigned int column;
  unsigned int height;
  unsigned int width;
  std::string formatting;
  unsigned int length;
  std::string text;

  RCT2Field(Interpreter::BytesType::const_iterator &position);

  std::string getLayoutString() const;
};
