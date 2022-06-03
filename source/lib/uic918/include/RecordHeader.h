#pragma once

#include <string>

#include "Interpreter.h"
#include "Utility.h"

struct RecordHeader
{
  Interpreter::BytesType::const_iterator const startPosition;
  std::string const recordId;
  std::string const recordVersion;
  unsigned int const recordLength;

  RecordHeader(Interpreter::BytesType::const_iterator &position)
      : startPosition(position),
        recordId(Utility::getAlphanumeric(position, 6)),
        recordVersion(Utility::getAlphanumeric(position, 2)),
        recordLength(std::stoi(Utility::getAlphanumeric(position, 4)))
  {
  }

  std::string to_string()
  {
    return recordId + ", " + recordVersion;
  }
};
