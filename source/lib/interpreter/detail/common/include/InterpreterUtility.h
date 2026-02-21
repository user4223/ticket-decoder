// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Context.h"

#include <vector>
#include <cstdint>

namespace interpreter::detail::common
{
  /* Consumes maximumSize or less bytes and returns a 0 terminated string.
     When the buffer is filled by multiple 0 at the end, the returned string might be shorter.
   */
  std::string consumeString(Context &context, std::size_t maximumSize);

  /* Consumes 4 bytes and converts from big-endian to system byte order
   */
  std::uint32_t consumeInteger4(Context &context);

  /* Consumes 3 bytes and converts from big-endian to system byte order
   */
  std::uint32_t consumeInteger3(Context &context);

  /* Consumes 2 bytes and converts from big-endian to system byte order
   */
  std::uint16_t consumeInteger2(Context &context);

  /* Consumes 1 byte
   */
  std::uint8_t consumeInteger1(Context &context);

  /* Consumes 4 bytes and decodes date-time to ISO-8601 format
   */
  std::string consumeDateTimeCompact4(Context &context);

  /* Consumes 3 bytes and decodes date-time to ISO-8601 format
   */
  std::string consumeDateTime3(Context &context);

  /* Consumes 8 bytes and decodes date to ISO-8601 format
   */
  std::string consumeDate8(Context &context);

  /***
     Converters
   ***/

  std::string bytesToString(std::span<std::uint8_t const> bytes);

  std::string bytesToHexString(std::span<std::uint8_t const> bytes);

  std::string bytesToHexString(std::vector<std::uint8_t> const &bytes);

  template <std::size_t S>
  std::string bytesToHexString(std::array<std::uint8_t, S> const &bytes)
  {
    return bytesToHexString(std::span<std::uint8_t const>(bytes.data(), S));
  }

  template <typename T>
  std::string bytesToHexString(T const &bytes)
  {
    auto const raw = std::span<std::uint8_t const>((std::uint8_t const *const)&bytes, sizeof(T));
    return std::endian::native == std::endian::big
               ? bytesToHexString(raw)
               : bytesToHexString(std::vector<std::uint8_t>(raw.rbegin(), raw.rend()));
  }
}
