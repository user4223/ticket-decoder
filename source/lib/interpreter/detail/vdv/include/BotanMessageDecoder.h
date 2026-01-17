// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "MessageDecoder.h"
#include "CertificateProvider.h"

#include "lib/infrastructure/include/Logger.h"

namespace interpreter::detail::vdv
{
    class BotanMessageDecoder : public MessageDecoder
    {
        infrastructure::Logger logger;
        CertificateProvider &certificateProvider;

    public:
        BotanMessageDecoder(infrastructure::LoggerFactory &loggerFactory, CertificateProvider &certificateProvider);

        /* Takes certificate from envelop and decodes the ticket certificate
           by using root + company (identified by authority) certificate.
         */
        virtual std::optional<std::vector<std::uint8_t>> decodeCertificate(
            std::span<std::uint8_t const> const &certificate,
            std::string const &authority) override;

        /* Decodes message from signature and ticket certificate.
         */
        virtual std::optional<std::vector<std::uint8_t>> decodeMessage(
            std::span<std::uint8_t const> const &signature,
            std::span<std::uint8_t const> const &residual,
            std::span<std::uint8_t const> const &certificate) override;
    };
}
