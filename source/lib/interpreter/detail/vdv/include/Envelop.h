// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <span>
#include <cstdint>

namespace interpreter::detail::vdv
{
    struct Envelop
    {
        std::string kennung;
        std::string version;
        std::string authority;
        std::span<std::uint8_t const> signature;
        std::span<std::uint8_t const> certificate;
    };
}
