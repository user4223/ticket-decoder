// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/RecordHeader.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include <algorithm>
#include <stdexcept>

namespace interpreter::detail::uic
{
  RecordHeader::RecordHeader(common::Context &context)
      : start(context.getPosition()),
        recordId(common::getAlphanumeric(context.getPosition(), 6)),
        recordVersion(common::getAlphanumeric(context.getPosition(), 2)),
        recordLength(std::stoi(common::getAlphanumeric(context.getPosition(), 4)))
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
