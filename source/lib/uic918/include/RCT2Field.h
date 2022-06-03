#pragma once

#include <string>

#include "Interpreter.h"

struct RCT2Field
{
  Interpreter::BytesType::const_iterator const startPosition;
  std::string fieldLine;
  std::string fieldColumn;
  std::string fieldHeight;
  std::string fieldWidth;
  std::string fieldFormatting;
  std::string fieldTextLength;
  std::string fieldText;
  unsigned int const recordLength;

  RCT2Field(Interpreter::BytesType::const_iterator &position);
};
