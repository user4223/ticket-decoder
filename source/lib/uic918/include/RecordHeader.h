#pragma once

#include "Context.h"

#include <string>
#include <vector>

struct RecordHeader
{
  std::string const recordId;
  std::string const recordVersion;
  unsigned int const recordLength;

  RecordHeader(Context &context);

  void ensure(std::string expectedRecordId, std::vector<std::string> expectedRecordVersions);

  std::string toString();
};
