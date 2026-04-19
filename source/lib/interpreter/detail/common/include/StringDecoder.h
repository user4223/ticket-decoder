// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <span>
#include <cstdint>
#include <vector>
#include <bit>
#include <array>

namespace interpreter::detail::common
{

    class Context;

    class StringDecoder
    {
    public:
        /* Consumes maximumBytes or less bytes (NOT chars) and returns a 0 terminated UTF8 string.
           When the buffer is filled by multiple 0 at the end, the returned string might be shorter.
           In case you are sure the buffer contains ASCII only, the method works fine and is the
           preferred method over consumeLatin1 since it avoids additional conversion.
         */
        static std::string consumeUTF8(Context &context, std::size_t maximumBytes);
        static std::string decodeUTF8(std::span<std::uint8_t const> bytes);

        /* Consumes maximumBytes or less ISO 8859-1 (Latin-1) encoded bytes and returns a 0
           terminated UTF8 string.
           When the buffer is filled by multiple 0 at the end, the returned string might be shorter.
         */
        static std::string consumeLatin1(Context &context, std::size_t maximumBytes);
        static std::string decodeLatin1(std::span<std::uint8_t const> bytes);

        static bool isASCII(std::span<std::uint8_t const> bytes, bool ensurePrintable);

        /* Consumes maximumBytes or less bytes and ensures it's plain ASCII (<128) and returns a
           0 terminated UTF8 string (which is the same as ASCII in this case).
         */
        static std::string consumeASCII(Context &context, std::size_t maximumBytes, bool ensurePrintable = false);
        static std::string decodeASCII(std::span<std::uint8_t const> bytes, bool ensurePrintable = false);

        /* Returns a string containing the hexadecimal representation of the given byte buffer.
         */
        static std::string toHexString(std::span<std::uint8_t const> bytes);
        static std::string toHexString(std::vector<std::uint8_t> const &bytes);

        template <std::size_t S>
        static std::string toHexString(std::array<std::uint8_t, S> const &bytes)
        {
            return toHexString(std::span<std::uint8_t const>(bytes.data(), S));
        }

        template <typename T>
        static std::string toHexString(T const &bytes)
        {
            auto const raw = std::span<std::uint8_t const>((std::uint8_t const *const)&bytes, sizeof(T));
            return std::endian::native == std::endian::big
                       ? toHexString(raw)
                       : toHexString(std::vector<std::uint8_t>(raw.rbegin(), raw.rend()));
        }
    };
}
