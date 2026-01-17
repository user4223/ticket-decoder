// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/infrastructure/include/Logging.h"

#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/x509cert.h>
#include <botan/auto_rng.h>
#include <botan/data_src.h>
#include <botan/rsa.h>

namespace interpreter::detail::vdv
{
    BotanMessageDecoder::BotanMessageDecoder(infrastructure::LoggerFactory &lf, CertificateProvider &cp)
        : logger(CREATE_LOGGER(lf)),
          certificateProvider(cp)
    {
    }

    Botan::RSA_PublicKey extractPublicKey(std::span<std::uint8_t const> const &certificate)
    {
        auto context = common::Context(certificate);
        auto const modulus = consumeExpectedTag(context, {0x5f, 0x37});
        auto const exponent = consumeExpectedTag(context, {0x5f, 0x38});
        ensureEmpty(context);

        // TODO This does not throw, but it might not be correct anyway
        return Botan::RSA_PublicKey(
            Botan::BigInt(modulus.data(), modulus.size()),
            Botan::BigInt(exponent.data(), exponent.size()));
    }

    Botan::RSA_PublicKey extractRootPublicKey(std::vector<std::uint8_t> const &element)
    {
        auto context = common::Context(element);
        auto payload = consumeExpectedTag(context, {0x7f, 0x21});
        ensureEmpty(context);

        auto payloadContext = common::Context(payload);
        auto a = consumeExpectedTag(payloadContext, {0x5f, 0x4e});
        auto b = consumeExpectedTag(payloadContext, {0x5f, 0x37});
        ensureEmpty(payloadContext);

        // TODO This does not throw, but it might not be correct anyway
        return Botan::RSA_PublicKey(
            Botan::BigInt(a.data(), a.size()),
            Botan::BigInt(b.data(), b.size()));
    }

    std::span<std::uint8_t const> extractUnknown(std::vector<std::uint8_t> const &element)
    {
        auto context = common::Context(element);
        auto payload = consumeExpectedTag(context, {0x7f, 0x21});
        ensureEmpty(context);

        auto payloadContext = common::Context(payload);
        auto a = consumeExpectedTag(payloadContext, {0x5f, 0x37});
        auto b = consumeExpectedTag(payloadContext, {0x5f, 0x38});
        ensureEmpty(payloadContext);

        // TODO Find out what a and b is
        return {};
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

        auto const rootCertificate = certificateProvider.get("4555564456100106");
        auto const companyCertificate = certificateProvider.get(authority);

        return std::nullopt;
    }

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decodeMessage(
        std::span<std::uint8_t const> const &signature,
        std::span<std::uint8_t const> const &residual,
        std::span<std::uint8_t const> const &certificate)
    {
        auto const publicKey = extractPublicKey(certificate);

        // auto decoder = Botan::PK_Decryptor_EME(*key, *rng, "RSA-1984(SHA-1)");
        // auto const message = decoder.decrypt(certificate.data(), certificate.size());

        return std::nullopt;
    }
}
