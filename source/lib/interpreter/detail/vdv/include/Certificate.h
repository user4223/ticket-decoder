// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <span>
#include <cstdint>
#include <string>

namespace interpreter::detail::vdv
{
    /* This object does not own the data, it's just a view.
     */
    struct Certificate
    {
        std::string authority;
        std::string description;
        std::span<std::uint8_t const> signature;
        std::span<std::uint8_t const> remainder;
        std::span<std::uint8_t const> content;
    };
}
