// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/NopCertificateProvider.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/infrastructure/include/Logging.h"

namespace interpreter::api
{
    NopCertificateProvider::NopCertificateProvider(infrastructure::Context &context)
        : logger(CREATE_LOGGER(context.getLoggerFactory()))
    {
        LOG_WARN(logger) << "Using dummy certificate provider";
    }

    std::unique_ptr<CertificateProvider> NopCertificateProvider::create(infrastructure::Context &context)
    {
        return std::make_unique<NopCertificateProvider>(context);
    }

    std::vector<std::string> NopCertificateProvider::getAuthorities()
    {
        return {};
    }

    std::optional<CertificateRawData> NopCertificateProvider::get(std::string authority)
    {
        return {};
    }
}
