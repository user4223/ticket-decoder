// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/NumberDecoder.h"

#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/Context.h"

#include <algorithm>
#include <bit>
#include <span>

namespace interpreter::detail::common
{

  template <typename T>
  T getInteger(std::span<std::uint8_t const> input, std::size_t length = sizeof(T))
  {
    if (input.size() < length)
    {
      throw std::runtime_error(std::string("Less than expected bytes available, expecting at least: ") + std::to_string(length));
    }

    auto const source = input.subspan(0, length);
    auto result = T();
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
  T getInteger(Context &context, std::size_t sourceLength = sizeof(T))
  {
    if (sizeof(T) < sourceLength)
    {
      throw std::runtime_error("Destination size must be equal or greater than source size");
    }

    return getInteger<T>(context.consumeBytes(sourceLength), sourceLength);
  }

  std::uint32_t NumberDecoder::consumeInteger4(Context &context)
  {
    return getInteger<std::uint32_t>(context);
  }

  std::string NumberDecoder::consumeInteger4AsString(Context &context)
  {
    return std::to_string(consumeInteger4(context));
  }

  std::uint32_t NumberDecoder::decodeInteger4(std::span<std::uint8_t const> source)
  {
    return getInteger<std::uint32_t>(source);
  }

  std::uint32_t NumberDecoder::consumeInteger3(Context &context)
  {
    return getInteger<std::uint32_t>(context, 3);
  }

  std::string NumberDecoder::consumeInteger3AsString(Context &context)
  {
    return std::to_string(consumeInteger3(context));
  }

  std::uint32_t NumberDecoder::decodeInteger3(std::span<std::uint8_t const> source)
  {
    return getInteger<std::uint32_t>(source, 3);
  }

  std::uint16_t NumberDecoder::consumeInteger2(Context &context)
  {
    return getInteger<std::uint16_t>(context);
  }

  std::string NumberDecoder::consumeInteger2AsString(Context &context)
  {
    return std::to_string(consumeInteger2(context));
  }

  std::uint16_t NumberDecoder::decodeInteger2(std::span<std::uint8_t const> source)
  {
    return getInteger<std::uint16_t>(source);
  }

  std::uint8_t NumberDecoder::consumeInteger1(Context &context)
  {
    return context.consumeByte();
  }

  std::string NumberDecoder::consumeInteger1AsString(Context &context)
  {
    return std::to_string(consumeInteger1(context));
  }

  std::uint8_t NumberDecoder::decodeInteger1(std::span<std::uint8_t const> source)
  {
    return getInteger<std::uint8_t>(source);
  }
}