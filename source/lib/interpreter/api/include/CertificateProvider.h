// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/infrastructure/include/ContextFwd.h"

#include "lib/interpreter/detail/vdv/include/Certificate.h" // TODO Replace and remove

#include <optional>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <span>

namespace interpreter::api
{

    /*struct CertificateData
    {
        std::string name;
        std::string description;
        std::span<std::uint8_t const> data;
    };*/

    class CertificateProvider
    {
    public:
        virtual ~CertificateProvider() = default;

        static std::unique_ptr<CertificateProvider> create(infrastructure::Context &context, std::filesystem::path const &vdvCertificateLdifFile);

        static std::unique_ptr<CertificateProvider> createDummy(infrastructure::Context &context);

        virtual std::vector<std::string> getAuthorities() = 0;

        /* TODO This should not use the type out of detail::vdv and should use a locally defined type, see above
         */
        virtual std::optional<detail::vdv::Certificate> get(std::string authority) = 0;
    };
}
