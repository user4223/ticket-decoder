// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/CertificateProvider.h"
#include "../include/NopCertificateProvider.h"

#include "lib/interpreter/detail/vdv/include/LDIFFileCertificateProvider.h"

#include "lib/infrastructure/include/Context.h"

namespace interpreter::api
{

    std::unique_ptr<CertificateProvider> CertificateProvider::create(infrastructure::Context &context, std::filesystem::path const &vdvCertificateLdifFile)
    {
        return std::make_unique<detail::vdv::LDIFFileCertificateProvider>(context, vdvCertificateLdifFile);
    }

    std::unique_ptr<CertificateProvider> CertificateProvider::createDummy(infrastructure::Context &context) 
    {
        return std::make_unique<NopCertificateProvider>(context);
    }
}
