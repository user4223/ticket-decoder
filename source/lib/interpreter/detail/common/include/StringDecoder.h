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
        /* Consumes maximumBytes or less bytes (NOT chars) and returns a 0 terminated string.
           When the buffer is filled by multiple 0 at the end, the returned string might be shorter.
         */
        static std::string consumeString(Context &context, std::size_t maximumBytes);

        static std::string bytesToString(std::span<std::uint8_t const> bytes);

        static std::string bytesToHexString(std::span<std::uint8_t const> bytes);

        static std::string bytesToHexString(std::vector<std::uint8_t> const &bytes);

        template <std::size_t S>
        static std::string bytesToHexString(std::array<std::uint8_t, S> const &bytes)
        {
            return bytesToHexString(std::span<std::uint8_t const>(bytes.data(), S));
        }

        template <typename T>
        static std::string bytesToHexString(T const &bytes)
        {
            auto const raw = std::span<std::uint8_t const>((std::uint8_t const *const)&bytes, sizeof(T));
            return std::endian::native == std::endian::big
                       ? bytesToHexString(raw)
                       : bytesToHexString(std::vector<std::uint8_t>(raw.rbegin(), raw.rend()));
        }
    };
}
