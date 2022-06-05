#pragma once

#include <string>

#include "Interpreter.h"

struct RCT2Field
{
  unsigned int fieldLine;
  unsigned int fieldColumn;
  unsigned int fieldHeight;
  unsigned int fieldWidth;
  std::string fieldFormatting;
  unsigned int fieldTextLength;
  std::string fieldText;

  RCT2Field(Interpreter::BytesType::const_iterator &position);

  std::string to_string();

  std::tuple<std::string, std::string> to_output(unsigned int index);
};
