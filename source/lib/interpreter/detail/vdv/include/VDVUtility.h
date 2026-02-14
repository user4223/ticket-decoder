// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Certificate.h"

#include "lib/interpreter/detail/common/include/TLVDecoder.h"

#include <array>
#include <cstdint>

namespace interpreter::detail::common
{
  class Context;
}

namespace interpreter::detail::vdv
{

  std::uint32_t consumeLength(common::Context &context);

  common::Context &consumeExpectedTag(common::Context &context, common::TLVTag const &expectedTag);

  common::Context &consumeExpectedEndTag(common::Context &context, common::TLVTag const &expectedTag);

  common::Context &consumeExpectedFrameTags(common::Context &context, common::TLVTag const &expectedBeginTag, common::TLVTag const &expectedEndTag);

  std::span<std::uint8_t const> consumeExpectedTagValue(common::Context &context, common::TLVTag const &expectedTag);

  std::span<std::uint8_t const> consumeExpected(common::Context &context, std::vector<std::uint8_t> expectedValue);

  bool peekExpected(common::Context &context, std::vector<std::uint8_t> expectedValue);

  void ensureTag(common::TLVTag const &tag, common::TLVTag const &expectedTag);
}
