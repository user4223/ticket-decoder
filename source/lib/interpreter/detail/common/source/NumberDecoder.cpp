// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/NumberDecoder.h"

#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/Context.h"

#include <algorithm>
#include <bit>
#include <span>
#include <charconv>
#include <limits>
#include <type_traits>

#if defined(__GNUC__) || defined(__clang__) // GCC / Clang
    #define bswap16 __builtin_bswap16
    #define bswap32 __builtin_bswap32
    #define bswap64 __builtin_bswap64
#else
    #error "Byte-swap intrinsics not supported on this compiler"
#endif

namespace interpreter::detail::common
{

  template <typename T>
  T toNativeEndianess(std::span<std::uint8_t const> source)
  {
    static_assert(std::is_integral_v<T>, "Integral types are supported only");

    // TODO Use std::byteswap when C++23 gets the lowest supported language version

    auto result = T(0);
    auto destination = std::span<std::uint8_t>(reinterpret_cast<std::uint8_t *>(&result), sizeof(T));

    if constexpr (std::endian::native == std::endian::big)
    {
      std::copy(source.begin(), source.end(), destination.begin() + sizeof(T) - source.size());
    }
    else
    {
      std::copy(source.rbegin(), source.rend(), destination.begin());
    }

    return result;
  }

  template <typename T>
  T getAtLeast(std::span<std::uint8_t const> input, std::size_t length = sizeof(T))
  {
    if (input.size() < length)
    {
      throw std::runtime_error(std::string("Less than expected bytes available, expecting at least: ") + std::to_string(length));
    }

    return toNativeEndianess<T>(input.subspan(0, length));
  }

  template <typename T>
  T getAtLeast(Context &context, std::size_t sourceLength = sizeof(T))
  {
    if (sizeof(T) < sourceLength)
    {
      throw std::runtime_error("Destination size must be equal or greater than source size");
    }

    return getAtLeast<T>(context.consumeBytes(sourceLength), sourceLength);
  }

  template <typename T>
  T getAtMost(std::span<std::uint8_t const> input, bool const isSigned, std::size_t length = sizeof(T))
  {
    if (input.size() > length)
    {
      throw std::runtime_error(std::string("More bytes available than expected, expecting at most: ") + std::to_string(length));
    } else if (input.empty())
    {
      throw std::runtime_error("No bytes available to decode");
    }

    auto const result = toNativeEndianess<T>(input);
    if (!isSigned)
    {
      return result;
    }

    auto const inputMask = std::make_unsigned_t<T>(0x80) << (input.size() - 1) * 8;
    auto const invertedInputMask = ~inputMask;
    auto const outputMask = ~std::make_unsigned_t<T>(0x80);// << (sizeof(T) - 1) * 8;
    
    auto const sign = (result & inputMask) != 0;
    auto const unsignedResult = result & invertedInputMask;
    auto const signedResult = unsignedResult | (sign ? outputMask : 0);
    return signedResult;
  }

  std::uint32_t NumberDecoder::decodeUInteger(std::string_view source)
  {
    auto value = std::uint32_t(0);
    std::from_chars(std::begin(source), std::end(source), value);
    return value;
  }

  std::int64_t NumberDecoder::decodeSInteger(std::span<std::uint8_t const> source)
  {
    return getAtMost<std::int64_t>(source, true);
  }

  std::uint32_t NumberDecoder::consumeUInteger4(Context &context)
  {
    return getAtLeast<std::uint32_t>(context);
  }

  std::string NumberDecoder::consumeUInteger4AsString(Context &context)
  {
    return std::to_string(consumeUInteger4(context));
  }

  std::uint32_t NumberDecoder::decodeUInteger4(std::span<std::uint8_t const> source)
  {
    return getAtLeast<std::uint32_t>(source);
  }

  std::uint32_t NumberDecoder::consumeUInteger3(Context &context)
  {
    return getAtLeast<std::uint32_t>(context, 3);
  }

  std::string NumberDecoder::consumeUInteger3AsString(Context &context)
  {
    return std::to_string(consumeUInteger3(context));
  }

  std::uint32_t NumberDecoder::decodeUInteger3(std::span<std::uint8_t const> source)
  {
    return getAtLeast<std::uint32_t>(source, 3);
  }

  std::uint16_t NumberDecoder::consumeUInteger2(Context &context)
  {
    return getAtLeast<std::uint16_t>(context);
  }

  std::string NumberDecoder::consumeUInteger2AsString(Context &context)
  {
    return std::to_string(consumeUInteger2(context));
  }

  std::uint16_t NumberDecoder::decodeUInteger2(std::span<std::uint8_t const> source)
  {
    return getAtLeast<std::uint16_t>(source);
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
    return getAtLeast<std::uint8_t>(source);
  }
}