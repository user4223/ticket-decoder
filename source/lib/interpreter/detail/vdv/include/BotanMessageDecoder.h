// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "MessageDecoder.h"
#include "Certificate.h"
#include "CertificateProvider.h"

#include "lib/infrastructure/include/LoggingFwd.h"

#include <memory>
#include <optional>
#include <map>

namespace interpreter::detail::vdv
{
    class BotanMessageDecoder : public MessageDecoder
    {
        class Internal;

        infrastructure::Logger logger;
        std::shared_ptr<Internal> internal;
        std::map<std::string, std::optional<DecodedCertificate>> issuingCertificates;

        std::optional<DecodedCertificate> getIssuingCertificate(std::string authority);

    public:
        BotanMessageDecoder(infrastructure::LoggerFactory &loggerFactory, CertificateProvider &certificateProvider);

        virtual std::optional<common::Context> decodeMessage(
            Certificate const &envelopeCertificate,
            Signature const &envelopeSignature) override;
    };
}
