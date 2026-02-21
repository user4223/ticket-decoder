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
    std::string StringDecoder::consumeString(Context &context, std::size_t size)
    {
        auto const data = context.consumeMaximalBytes(size);
        return bytesToString(data);
    }

    std::string StringDecoder::bytesToString(std::span<std::uint8_t const> bytes)
    {
        // auto ascii = std::vector<std::uint8_t>();
        // std::transform(std::begin(bytes), std::end(bytes), std::back_inserter(ascii), [](std::uint8_t const &v)
        //               { return v >= 128 ? ' ' : v; });
        auto result = std::string{std::begin(bytes), std::find(std::begin(bytes), std::end(bytes), '\0')};
        result.erase(std::find_if(std::rbegin(result), std::rend(result), [](unsigned char ch)
                                  { return !std::isspace(ch); })
                         .base(),
                     std::end(result));
        return result;
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
