
#include "../include/RecordHeader.h"

#include "lib/interpreter/detail/common/include/Utility.h"

#include <algorithm>
#include <stdexcept>

namespace interpreter::detail::uic
{
  RecordHeader::RecordHeader(Context &context)
      : start(context.getPosition()),
        recordId(getAlphanumeric(context.getPosition(), 6)),
        recordVersion(getAlphanumeric(context.getPosition(), 2)),
        recordLength(std::stoi(getAlphanumeric(context.getPosition(), 4)))
  {
    context.addField(recordId + ".recordId", recordId);
    context.addField(recordId + ".recordVersion", recordVersion);
    context.addField(recordId + ".recordLength", std::to_string(recordLength));

    auto const entry = context.getField("recordIds");
    context.addField("recordIds", entry ? entry->getValue() + " " + recordId : recordId);
  }

  void RecordHeader::ensure(std::string expectedRecordId, std::vector<std::string> expectedRecordVersions)
  {
    if (recordId.compare(expectedRecordId) != 0 ||
        std::find(expectedRecordVersions.begin(), expectedRecordVersions.end(), recordVersion) == expectedRecordVersions.end())
    {
      throw std::runtime_error(std::string("Unsupported header: ") + toString());
    }
  }

  std::size_t RecordHeader::getRemaining(std::vector<std::uint8_t>::const_iterator const current)
  {
    return recordLength - std::distance(start, current);
  }

  std::string RecordHeader::toString()
  {
    return recordId + ", " + recordVersion;
  }
}
