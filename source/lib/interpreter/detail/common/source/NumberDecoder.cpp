// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/NumberDecoder.h"

#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/Context.h"

#include <algorithm>
#include <bit>
#include <span>
#include <charconv>
#include <type_traits>

namespace interpreter::detail::common
{
  template <typename T>
  T getUnsignedAtLeast(std::span<std::uint8_t const> input, std::size_t length = sizeof(T))
  {
    static_assert(std::is_integral_v<T>, "Integral types are supported only");
    static_assert(std::is_unsigned_v<T>, "Unsigned types are supported only");
    static_assert(std::endian::native == std::endian::little, "Little endian arch are supported only");

    if (input.size() < length)
    {
      throw std::runtime_error(std::string("Less than expected bytes available, expecting at least: ") + std::to_string(length));
    }

    auto target = T(0);
    auto destination = std::span<std::uint8_t>(reinterpret_cast<std::uint8_t *>(&target), sizeof(T));
    auto const source = input.subspan(0, length);

    // if constexpr (std::endian::native == std::endian::big)
    // {
    //   auto const offset = sizeof(T) - input.size();
    //   std::copy(std::begin(input), std::end(input), std::begin(destination) + offset);
    // }
    // else
    {
      std::copy(std::rbegin(source), std::rend(source), std::begin(destination));
    }

    return target;
  }

  template <typename T>
  T consumeUnsigned(Context &context, std::size_t sourceLength = sizeof(T))
  {
    if (sizeof(T) < sourceLength)
    {
      throw std::runtime_error("Destination size must be equal or greater than source size");
    }

    return getUnsignedAtLeast<T>(context.consumeBytes(sourceLength), sourceLength);
  }

  template <typename T>
  T getSignedAtMost(std::span<std::uint8_t const> input)
  {
    static_assert(std::is_integral_v<T>, "Integral types are supported only");
    static_assert(std::is_signed_v<T>, "Signed types are supported only");
    static_assert(std::endian::native == std::endian::little, "Little endian arch are supported only");

    if (input.size() > sizeof(T))
    {
      throw std::runtime_error(std::string("More bytes available than expected, expecting at most: ") + std::to_string(sizeof(T)));
    }
    else if (input.empty())
    {
      throw std::runtime_error("No bytes available to decode");
    }

    auto const signByte = std::uint8_t(input[0] & 0x80 ? 0xff : 0x00);
    auto const offset = sizeof(T) - input.size();

    auto target = T(0);
    auto destination = std::span<std::uint8_t>(reinterpret_cast<std::uint8_t *>(&target), sizeof(T));

    // if constexpr (std::endian::native == std::endian::big)
    // {
    //   std::fill_n(std::begin(destination) + input.size(), offset, signByte);
    //   std::copy(std::begin(input), std::end(input), std::begin(destination) + offset);
    // }
    // else
    {
      std::fill_n(std::begin(destination) + input.size(), offset, signByte);
      std::copy(std::rbegin(input), std::rend(input), std::begin(destination));
    }

    return target;
  }

  std::uint32_t NumberDecoder::decodeUInteger(std::string_view source)
  {
    auto value = std::uint32_t(0);
    std::from_chars(std::begin(source), std::end(source), value);
    return value;
  }

  std::int64_t NumberDecoder::decodeSInteger(std::span<std::uint8_t const> source)
  {
    return getSignedAtMost<std::int64_t>(source);
  }

  std::uint32_t NumberDecoder::consumeUInteger4(Context &context)
  {
    return consumeUnsigned<std::uint32_t>(context);
  }

  std::string NumberDecoder::consumeUInteger4AsString(Context &context)
  {
    return std::to_string(consumeUInteger4(context));
  }

  std::uint32_t NumberDecoder::decodeUInteger4(std::span<std::uint8_t const> source)
  {
    return getUnsignedAtLeast<std::uint32_t>(source);
  }

  std::uint32_t NumberDecoder::consumeUInteger3(Context &context)
  {
    return consumeUnsigned<std::uint32_t>(context, 3);
  }

  std::string NumberDecoder::consumeUInteger3AsString(Context &context)
  {
    return std::to_string(consumeUInteger3(context));
  }

  std::uint32_t NumberDecoder::decodeUInteger3(std::span<std::uint8_t const> source)
  {
    return getUnsignedAtLeast<std::uint32_t>(source, 3);
  }

  std::uint16_t NumberDecoder::consumeUInteger2(Context &context)
  {
    return consumeUnsigned<std::uint16_t>(context);
  }

  std::string NumberDecoder::consumeUInteger2AsString(Context &context)
  {
    return std::to_string(consumeUInteger2(context));
  }

  std::uint16_t NumberDecoder::decodeUInteger2(std::span<std::uint8_t const> source)
  {
    return getUnsignedAtLeast<std::uint16_t>(source);
  }

  std::uint8_t NumberDecoder::consumeUInteger1(Context &context)
  {
    return context.consumeByte();
  }

  std::string NumberDecoder::consumeUInteger1AsString(Context &context)
  {
    return std::to_string(consumeUInteger1(context));
  }

  std::uint8_t NumberDecoder::decodeUInteger1(std::span<std::uint8_t const> source)
  {
    return getUnsignedAtLeast<std::uint8_t>(source);
  }
}