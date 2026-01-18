// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Certificate.h"

#include <optional>
#include <string>
#include <vector>

namespace interpreter::detail::vdv
{
    class CertificateProvider
    {
    public:
        virtual ~CertificateProvider() = default;

        virtual std::vector<std::string> getAuthorities() = 0;

        virtual std::optional<Certificate> getRoot() = 0;

        virtual std::optional<Certificate> get(std::string authority) = 0;
    };
}
