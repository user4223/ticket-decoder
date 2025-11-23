// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace utility::base64
{

  std::vector<std::uint8_t> decode(std::string const &in);

  std::string encode(std::vector<std::uint8_t> const &in);

  std::string encode(std::uint8_t const *const data, size_t size);

}
