#pragma once

#include "Context.h"

#include <string>

struct RCT2Field
{
  unsigned int line;
  unsigned int column;
  unsigned int height;
  unsigned int width;
  std::string formatting;
  unsigned int length;
  std::string text;

  RCT2Field(Context::BytesType::const_iterator &position);

  std::string getLayoutString() const;
};
