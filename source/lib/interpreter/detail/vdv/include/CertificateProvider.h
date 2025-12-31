
// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>
#include <cstdint>
#include <string>

namespace interpreter::detail::vdv
{
    struct Certificate
    {
        std::string commonName;
        std::string distinguishedName;
        std::string description;
        std::vector<std::uint8_t> certificate;
    };

    class CertificateProvider
    {
    public:
        virtual ~CertificateProvider() = default;

        virtual std::optional<Certificate> get(std::string commonName) = 0;
    };
}
