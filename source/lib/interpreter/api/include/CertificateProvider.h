// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/infrastructure/include/ContextFwd.h"

#include <optional>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <span>

namespace interpreter::api
{

    struct CertificateRawData
    {
        std::string name;
        std::string description;
        std::span<std::uint8_t const> data;
    };

    class CertificateProvider
    {
    public:
        virtual ~CertificateProvider() = default;

        static std::unique_ptr<CertificateProvider> create(infrastructure::Context &context, std::filesystem::path const &vdvCertificateLdifFile);

        static std::unique_ptr<CertificateProvider> createDummy(infrastructure::Context &context);

        virtual std::vector<std::string> getAuthorities() = 0;

        virtual std::optional<CertificateRawData> get(std::string authority) = 0;
    };
}
