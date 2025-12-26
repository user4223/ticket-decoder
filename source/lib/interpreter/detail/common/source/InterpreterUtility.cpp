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
  std::string getAlphanumeric(Context &context, std::size_t size)
  {
    auto const data = context.consumeMaximalBytes(size);
    auto result = std::string{std::begin(data), std::find(std::begin(data), std::end(data), '\0')};
    result.erase(std::find_if(std::rbegin(result), std::rend(result), [](unsigned char ch)
                              { return !std::isspace(ch); })
                     .base(),
                 std::end(result));
    return result;
  }

  template <typename T>
  T getNumeric(Context &context, std::size_t sourceLength = sizeof(T))
  {
    if (sizeof(T) < sourceLength)
    {
      throw std::runtime_error("Destination size must be equal or greater than source size");
    }

    auto source = context.consumeBytes(sourceLength);
    auto destination = std::vector<std::uint8_t>(sizeof(T), 0);

    // TODO This depends on endianess, call revers only when endianess change between source and destination
    // if constexpr (std::endian::native == std::endian::big)
    // {
    //   throw std::runtime_error("Big endian machines not supported right now");
    // }
    std::reverse(source.begin(), source.end()); // TODO Big endian do not has to revert byte order
    auto const offset = 0;                      // destination.size() - source.size(); // TODO Big endian has to offset here

    std::copy(source.begin(), source.end(), destination.begin() + offset);
    return *(reinterpret_cast<T const *>(destination.data()));
  }

  std::uint32_t getNumeric32(Context &context)
  {
    return getNumeric<std::uint32_t>(context);
  }

  std::uint32_t getNumeric24(Context &context)
  {
    return getNumeric<std::uint32_t>(context, 3);
  }

  std::uint16_t getNumeric16(Context &context)
  {
    return getNumeric<std::uint16_t>(context);
  }

  std::uint8_t getNumeric8(Context &context)
  {
    return getNumeric<std::uint8_t>(context);
  }

  std::string getDateTimeCompact(Context &context)
  {
    auto const date = common::getNumeric16(context);
    auto const time = common::getNumeric16(context);
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

  std::string getDateTime12(Context &context)
  {
    auto const input = getAlphanumeric(context, 12);
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

  std::string getDate8(Context &context)
  {
    auto const input = getAlphanumeric(context, 8);
    auto const p = input.begin();
    // TODO Use chrono parse or apply validation for all values
    std::ostringstream os; // DDMMYYYY
    os << std::string(p + 4, p + 8) << "-"
       << std::string(p + 2, p + 4) << "-"
       << std::string(p + 0, p + 2);
    return os.str();
  }

  std::string bytesToString(std::vector<std::uint8_t> const &typeId)
  {
    if (typeId.empty())
    {
      return "";
    }

    std::stringstream os;
    os << "0x";
    std::for_each(std::begin(typeId), std::end(typeId), [&](auto const &byte)
                  { os << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)byte; });
    return os.str();
  }
}