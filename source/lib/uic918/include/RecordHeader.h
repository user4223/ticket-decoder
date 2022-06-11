#pragma once

#include <string>

#include "Interpreter.h"
#include "Utility.h"

struct RecordHeader
{
  std::string const recordId;
  std::string const recordVersion;
  unsigned int const recordLength;

  RecordHeader(Context::BytesType::const_iterator &position)
      : recordId(Utility::getAlphanumeric(position, 6)),
        recordVersion(Utility::getAlphanumeric(position, 2)),
        recordLength(std::stoi(Utility::getAlphanumeric(position, 4)))
  {
  }

  std::string toString()
  {
    return recordId + ", " + recordVersion;
  }
};
