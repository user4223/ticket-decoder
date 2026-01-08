// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/detail/common/include/Context.h"

#include <array>
#include <cstdint>

namespace interpreter::detail::vdv
{

  using TagType = std::array<std::uint8_t, 2>;

  std::uint32_t getLength(common::Context &context);

  TagType getTag(common::Context &context);

  common::Context &ensureExpectedTag(common::Context &context, TagType expectedTag);

  std::span<std::uint8_t const> consumeExpectedTag(common::Context &context, TagType expectedTag);

  void ensureEmpty(common::Context const &context);
}
