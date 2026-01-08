// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>
#include <cstdint>
#include <span>
#include <string>

namespace interpreter::detail::vdv
{
    class MessageDecoder
    {
    public:
        virtual ~MessageDecoder() = default;

        virtual std::optional<std::vector<std::uint8_t>> decode(std::span<std::uint8_t const> const &ticketCertificate, std::string authority) = 0;
    };
}
