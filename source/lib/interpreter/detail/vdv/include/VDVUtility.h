// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Certificate.h"

#include "lib/interpreter/detail/common/include/Context.h"

#include <array>
#include <cstdint>

namespace interpreter::detail::vdv
{

  using TagType = std::array<std::uint8_t, 2>;

  std::uint32_t consumeLength(common::Context &context);

  TagType consumeTag(common::Context &context);

  common::Context &consumeExpectedTag(common::Context &context, TagType const &expectedTag);

  common::Context &consumeExpectedEndTag(common::Context &context, TagType const &expectedTag);

  common::Context &consumeExpectedFrameTags(common::Context &context, TagType const &expectedBeginTag, TagType const &expectedEndTag);

  std::span<std::uint8_t const> consumeExpectedTagValue(common::Context &context, TagType const &expectedTag);

  std::span<std::uint8_t const> consumeExpected(common::Context &context, std::vector<std::uint8_t> expectedValue);

  bool peekExpected(common::Context &context, std::vector<std::uint8_t> expectedValue);

  void ensureTag(TagType const &tag, TagType const &expectedTag);

  void ensureEmpty(common::Context const &context);
}
