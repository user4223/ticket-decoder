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

  void RecordHeader::ensure(std::string expectedRecordId, std::vector<std::string> expectedRecordVersions) const
  {
    if (recordId.compare(expectedRecordId) != 0 ||
        std::find(expectedRecordVersions.begin(), expectedRecordVersions.end(), recordVersion) == expectedRecordVersions.end())
    {
      throw std::runtime_error(std::string("Unsupported header: ") + toString());
    }
  }

  std::size_t RecordHeader::getRemainingRecordSize(common::Context &context) const
  {
    auto const recordConsumed = std::distance(start, context.getPosition());
    if (recordConsumed < 0)
    {
      throw std::runtime_error("Illegal state in header detected, current position seems to be before header begin");
    }

    auto const recordRemaining = recordLength - recordConsumed;
    if (context.getRemainingSize() < recordRemaining)
    {
      throw std::runtime_error("Not enough overall bytes remaining to consume record");
    }

    return recordRemaining;
  }

  std::vector<std::uint8_t> RecordHeader::consumeRecordBytes(common::Context &context)
  {
    return context.consumeBytes(getRemainingRecordSize(context));
  }

  std::size_t RecordHeader::ignoreRecordBytes(common::Context &context)
  {
    return context.ignoreBytes(getRemainingRecordSize(context));
  }

  std::string RecordHeader::toString() const
  {
    return recordId + ", " + recordVersion;
  }
}
