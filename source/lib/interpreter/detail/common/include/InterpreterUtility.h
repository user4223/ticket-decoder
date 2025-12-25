// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Context.h"

#include <vector>
#include <cstdint>

namespace interpreter::detail::common
{
  std::string getAlphanumeric(Context &context, std::size_t size);

  std::uint32_t getNumeric32(Context &context);

  std::uint32_t getNumeric24(Context &context);

  std::uint16_t getNumeric16(Context &context);

  std::uint8_t getNumeric8(Context &context);

  std::string getDateTimeCompact(Context &context);

  std::string getDateTime12(Context &context);

  std::string getDate8(Context &context);

  std::string bytesToString(std::vector<std::uint8_t> const &bytes);
}
