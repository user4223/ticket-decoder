// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/InterpreterUtility.h"

#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/Context.h"

#include <algorithm>
#include <bit>
#include <span>

namespace interpreter::detail::common
{

  template <typename T>
  T getInteger(Context &context, std::size_t sourceLength = sizeof(T))
  {
    if (sizeof(T) < sourceLength)
    {
      throw std::runtime_error("Destination size must be equal or greater than source size");
    }

    auto const source = context.consumeBytes(sourceLength);
    auto result = T();
    auto destination = std::span<std::uint8_t>(reinterpret_cast<std::uint8_t *>(&result), sizeof(T));

    if constexpr (std::endian::native == std::endian::big)
    {
      std::copy(source.begin(), source.end(), destination.begin() + sizeof(T) - sourceLength);
    }
    else
    {
      std::copy(source.rbegin(), source.rend(), destination.begin());
    }

    return result;
  }

  std::uint32_t NumberDecoder::consumeInteger4(Context &context)
  {
    return getInteger<std::uint32_t>(context);
  }

  std::uint32_t NumberDecoder::consumeInteger3(Context &context)
  {
    return getInteger<std::uint32_t>(context, 3);
  }

  std::uint16_t NumberDecoder::consumeInteger2(Context &context)
  {
    return getInteger<std::uint16_t>(context);
  }

  std::uint8_t NumberDecoder::consumeInteger1(Context &context)
  {
    return context.consumeByte();
  }
}