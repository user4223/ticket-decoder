// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/StringDecoder.h"

#include "lib/interpreter/detail/common/include/Context.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>

namespace interpreter::detail::common
{
    static std::string latin1ToUtf8(std::string &&latin1)
    {
        auto utf8 = std::string{};
        utf8.reserve(latin1.capacity());
        for (std::uint8_t const &ch : latin1)
        {
            if (ch < 0x80)
            {
                utf8.push_back(ch);
            }
            else
            {
                utf8.push_back(0xc0 | ch >> 6);
                utf8.push_back(0x80 | (ch & 0x3f));
            }
        }
        return utf8;
    }

    static constexpr std::string toString(std::span<std::uint8_t const> bytes)
    {
        auto const first = std::begin(bytes);
        return std::string{first, std::find(first, std::end(bytes), '\0')};
    }

    static std::string removeTrailingSpaces(std::string &&input)
    {
        auto const last = std::rbegin(input);
        auto const position = std::find_if(last, std::rend(input), [](unsigned char ch)
                                           { return !std::isspace(ch); })
                                  .base();

        auto const end = std::end(input);
        if (position != end)
        {
            input.erase(position, end);
        }
        return input;
    }

    std::string StringDecoder::consumeUTF8(Context &context, std::size_t maximumBytes)
    {
        auto const data = context.consumeMaximalBytes(maximumBytes);
        return decodeUTF8(data);
    }

    std::string StringDecoder::decodeUTF8(std::span<std::uint8_t const> bytes)
    {
        return removeTrailingSpaces(toString(bytes));
    }

    std::string StringDecoder::consumeLatin1(Context &context, std::size_t maximumBytes)
    {
        auto const data = context.consumeMaximalBytes(maximumBytes);
        return decodeLatin1(data);
    }

    std::string StringDecoder::decodeLatin1(std::span<std::uint8_t const> bytes)
    {
        return removeTrailingSpaces(latin1ToUtf8(toString(bytes)));
    }

    std::string StringDecoder::bytesToHexString(std::span<std::uint8_t const> bytes)
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

    std::string StringDecoder::bytesToHexString(std::vector<std::uint8_t> const &bytes)
    {
        return bytesToHexString(std::span<std::uint8_t const>(bytes.data(), bytes.size()));
    }
}
