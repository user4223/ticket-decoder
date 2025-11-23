// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>
#include <cstdint>

namespace interpreter::detail::common
{
  std::vector<std::uint8_t> deflate(std::vector<std::uint8_t> const &compressed);
}
