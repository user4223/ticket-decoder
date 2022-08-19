#pragma once

#include "Context.h"

#include <string>
#include <vector>

namespace uic918::detail
{
  struct RecordHeader
  {
    Context::BytesType::const_iterator const start;
    std::string const recordId;
    std::string const recordVersion;
    unsigned int const recordLength;

    RecordHeader(Context &context);

    void ensure(std::string expectedRecordId, std::vector<std::string> expectedRecordVersions);

    std::size_t getRemaining(Context::BytesType::const_iterator position);

    std::string toString();
  };
}
