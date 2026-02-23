// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>

namespace interpreter::detail::common
{
  class Context;

  class NumberDecoder
  {
  public:
    /* Consumes 4 bytes and converts from big-endian to system byte order
     */
    static std::uint32_t consumeInteger4(Context &context);

    /* Consumes 3 bytes and converts from big-endian to system byte order
     */
    static std::uint32_t consumeInteger3(Context &context);

    /* Consumes 2 bytes and converts from big-endian to system byte order
     */
    static std::uint16_t consumeInteger2(Context &context);

    /* Consumes 1 byte
     */
    static std::uint8_t consumeInteger1(Context &context);
  };
}
