// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

/* TODO Rename to converter
 */
namespace interpreter::detail::common
{
  std::string getAlphanumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t size);

  std::uint32_t getNumeric32(std::vector<std::uint8_t>::const_iterator &position);

  std::uint32_t getNumeric24(std::vector<std::uint8_t>::const_iterator &position);

  std::uint16_t getNumeric16(std::vector<std::uint8_t>::const_iterator &position);

  std::uint8_t getNumeric8(std::vector<std::uint8_t>::const_iterator &position);

  std::string getDateTimeCompact(std::vector<std::uint8_t>::const_iterator &position);

  std::string getDateTime12(std::vector<std::uint8_t>::const_iterator &position);

  std::string getDate8(std::vector<std::uint8_t>::const_iterator &position);

  std::vector<std::uint8_t> getBytes(std::vector<std::uint8_t>::const_iterator &position, std::size_t size);
}
