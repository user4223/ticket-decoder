// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/detail/common/include/Context.h"

#include <string>
#include <vector>
#include <span>

namespace interpreter::detail::uic
{
  struct RecordHeader
  {
    std::vector<std::uint8_t>::const_iterator const start;
    std::string const recordId;
    std::string const recordVersion;
    unsigned int const recordLength;

    RecordHeader(common::Context &context);

    void ensure(std::string expectedRecordId, std::vector<std::string> expectedRecordVersions) const;

    std::size_t getRemainingRecordSize(common::Context &context) const;

    std::span<std::uint8_t const> consumeRemainingRecordBytes(common::Context &context);

    std::size_t ignoreRemainingRecordBytes(common::Context &context);

    std::string toString() const;
  };
}
