// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>
#include <span>
#include <string>

namespace interpreter::detail::common
{
  class Context;

  class NumberDecoder
  {
  public:
    static std::uint32_t decodeUInteger(std::string_view source);

    static std::int64_t decodeSInteger(std::span<std::uint8_t const> source);

    /* Consumes 4 bytes and converts from big-endian to system byte order
     */
    static std::uint32_t consumeUInteger4(Context &context);
    static std::string consumeUInteger4AsString(Context &context);
    static std::uint32_t decodeUInteger4(std::span<std::uint8_t const> source);

    /* Consumes 3 bytes and converts from big-endian to system byte order
     */
    static std::uint32_t consumeUInteger3(Context &context);
    static std::string consumeUInteger3AsString(Context &context);
    static std::uint32_t decodeUInteger3(std::span<std::uint8_t const> source);

    /* Consumes 2 bytes and converts from big-endian to system byte order
     */
    static std::uint16_t consumeUInteger2(Context &context);
    static std::string consumeUInteger2AsString(Context &context);
    static std::uint16_t decodeUInteger2(std::span<std::uint8_t const> source);

    /* Consumes 1 byte
     */
    static std::uint8_t consumeUInteger1(Context &context);
    static std::string consumeUInteger1AsString(Context &context);
    static std::uint8_t decodeUInteger1(std::span<std::uint8_t const> source);
  };
}
