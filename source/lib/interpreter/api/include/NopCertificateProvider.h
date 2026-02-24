// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "CertificateProvider.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/Logger.h"

namespace interpreter::api
{

    class NopCertificateProvider : public CertificateProvider
    {
        infrastructure::Logger logger;

    public:
        NopCertificateProvider(infrastructure::Context &context);

        static std::unique_ptr<CertificateProvider> create(infrastructure::Context &context);

        virtual std::vector<std::string> getAuthorities() override;

        virtual std::optional<detail::vdv::Certificate> get(std::string authority) override;
    };
}
