// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/infrastructure/include/Logging.h"

#ifdef WITH_SIGNATURE_VERIFIER
#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/x509cert.h>
#include <botan/der_enc.h>
#include <botan/auto_rng.h>
#include <botan/data_src.h>
#include <botan/rsa.h>
#endif

namespace interpreter::detail::vdv
{
    BotanMessageDecoder::BotanMessageDecoder(infrastructure::LoggerFactory &lf, CertificateProvider &cp)
        : logger(CREATE_LOGGER(lf)),
          certificateProvider(cp)
    {
    }

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decodeCertificate(
        std::span<std::uint8_t const> const &certificate,
        std::string const &authority)
    {
        // TODO
        //  - Identify and get authority certificate by using authority
        //  - Get root certificate
        //  - Decode authority certificate by using root certificate
        //  - Decode ticket certificate by using given certificate + authority certificate
        //  - Return ticket certificate

        auto const rootCertificate = certificateProvider.getRoot();
        if (!rootCertificate)
        {
            return std::nullopt;
        }
        auto const companyCertificate = certificateProvider.get(authority);
        if (!companyCertificate)
        {
            return std::nullopt;
        }

        auto context = common::Context(certificate);
        auto const ticketSignature = consumeExpectedTag(context, {0x5f, 0x37});
        auto const ticketRemainder = consumeExpectedTag(context, {0x5f, 0x38});
        ensureEmpty(context);

        // auto dataSource = Botan::DataSource_Memory(rootCertificate->content);
        // auto cert = Botan::X509_Certificate(dataSource);
        // Botan::X509_Certificate(rootCertificate->)

        // auto decoder = Botan::PK_Decryptor_EME(*key, *rng, "RSA-1984(SHA-1)");
        // auto const message = decoder.decrypt(certificate.data(), certificate.size());

        return std::nullopt;
    }

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decodeMessage(
        std::span<std::uint8_t const> const &signature,
        std::span<std::uint8_t const> const &residual,
        std::span<std::uint8_t const> const &certificate)
    {

        return std::nullopt;
    }
}
