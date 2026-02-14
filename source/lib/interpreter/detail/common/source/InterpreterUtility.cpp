// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/InterpreterUtility.h"

#include <algorithm>
#include <bit>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <array>
#include <iterator>

namespace interpreter::detail::common
{
  std::string consumeString(Context &context, std::size_t size)
  {
    auto const data = context.consumeMaximalBytes(size);
    return bytesToString(data);
  }

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
      std::copy(source.begin(), source.end(), destination.begin());
    }
    else
    {
      std::copy(source.rbegin(), source.rend(), destination.begin());
    }

    return result;
  }

  std::uint32_t consumeInteger4(Context &context)
  {
    return getInteger<std::uint32_t>(context);
  }

  std::uint32_t consumeInteger3(Context &context)
  {
    return getInteger<std::uint32_t>(context, 3);
  }

  std::uint16_t consumeInteger2(Context &context)
  {
    return getInteger<std::uint16_t>(context);
  }

  std::uint8_t consumeInteger1(Context &context)
  {
    return context.consumeByte();
  }

  std::uint16_t consumeDecimalInteger2(Context &context)
  {
    std::uint16_t const high = consumeDecimalInteger1(context);
    std::uint16_t const low = consumeDecimalInteger1(context);
    return high * 100 + low;
  }

  std::uint8_t consumeDecimalInteger1(Context &context)
  {
    auto byte = context.consumeByte();
    std::uint8_t const high = byte >> 4 & 0x0F;
    std::uint8_t const low = byte & 0x0F;
    return high * 10 + low;
  }

  std::string consumeDateTimeCompact4(Context &context)
  {
    auto const date = common::consumeInteger2(context);
    auto const time = common::consumeInteger2(context);
    // TODO Use chrono parse or apply validation for all values
    std::ostringstream os;
    os << std::setw(4) << std::setfill('0') << std::to_string(((date & 0xFE00) >> 9) + 1990) << "-"
       << std::setw(2) << std::setfill('0') << std::to_string(((date & 0x01E0) >> 5)) << "-"
       << std::setw(2) << std::setfill('0') << std::to_string(((date & 0x001F) >> 0)) << "T"
       << std::setw(2) << std::setfill('0') << std::to_string(((time & 0xF800) >> 11)) << ":"
       << std::setw(2) << std::setfill('0') << std::to_string(((time & 0x07E0) >> 5)) << ":"
       << std::setw(2) << std::setfill('0') << std::to_string(((time & 0x001F) >> 0));
    return os.str();
  }

  std::string consumeDateTime3(Context &context)
  {
    auto const input = consumeString(context, 12);
    auto const p = input.begin();
    // TODO Use chrono parse or apply validation for all values
    std::ostringstream os; // DDMMYYYYHHMM
    os << std::string(p + 4, p + 8) << "-"
       << std::string(p + 2, p + 4) << "-"
       << std::string(p + 0, p + 2) << "T"
       << std::string(p + 8, p + 10) << ":"
       << std::string(p + 10, p + 12) << ":"
       << "00";
    return os.str();
  }

  std::string consumeDate8(Context &context)
  {
    auto const input = consumeString(context, 8);
    auto const p = input.begin();
    // TODO Use chrono parse or apply validation for all values
    std::ostringstream os; // DDMMYYYY
    os << std::string(p + 4, p + 8) << "-"
       << std::string(p + 2, p + 4) << "-"
       << std::string(p + 0, p + 2);
    return os.str();
  }

  std::string bytesToString(std::span<std::uint8_t const> bytes)
  {
    auto result = std::string{std::begin(bytes), std::find(std::begin(bytes), std::end(bytes), '\0')};
    result.erase(std::find_if(std::rbegin(result), std::rend(result), [](unsigned char ch)
                              { return !std::isspace(ch); })
                     .base(),
                 std::end(result));
    return result;
  }

  std::string bytesToHexString(std::span<std::uint8_t const> bytes)
  {
    if (bytes.empty())
    {
      return "";
    }

    std::stringstream os;
    std::for_each(std::begin(bytes), std::end(bytes), [&](auto const &byte)
                  { os << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)byte; });
    return os.str();
  }

  std::string bytesToHexString(std::vector<std::uint8_t> const &bytes)
  {
    return bytesToHexString(std::span<std::uint8_t const>(bytes.data(), bytes.size()));
  }
}