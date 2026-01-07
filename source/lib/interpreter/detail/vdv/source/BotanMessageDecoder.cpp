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

    Botan::RSA_PublicKey getPublicKey(std::span<std::uint8_t const> const &certificate)
    {
        auto context = common::Context(certificate);
        auto const modulus = context.consumeBytes(getLength(ensureExpectedTag(context, {0x5f, 0x37})));
        auto const exponent = context.consumeBytes(getLength(ensureExpectedTag(context, {0x5f, 0x38})));
        ensureEmpty(context);

        // TODO This does not throw, but it might not be correct anyway
        return Botan::RSA_PublicKey(
            Botan::BigInt(modulus.data(), modulus.size()),
            Botan::BigInt(exponent.data(), exponent.size()));
    }

    Botan::RSA_PublicKey getRootKey(std::vector<std::uint8_t> const &element)
    {
        auto context = common::Context(element);
        auto payload = context.consumeBytes(getLength(ensureExpectedTag(context, {0x7f, 0x21})));
        ensureEmpty(context);

        auto payloadContext = common::Context(payload);
        auto a = payloadContext.consumeBytes(getLength(ensureExpectedTag(payloadContext, {0x5f, 0x4e})));
        auto b = payloadContext.consumeBytes(getLength(ensureExpectedTag(payloadContext, {0x5f, 0x37})));
        ensureEmpty(payloadContext);

        // TODO This does not throw, but it might not be correct anyway
        return Botan::RSA_PublicKey(
            Botan::BigInt(a.data(), a.size()),
            Botan::BigInt(b.data(), b.size()));
    }

    std::span<std::uint8_t const> getCompanyKey(std::vector<std::uint8_t> const &element)
    {
        auto context = common::Context(element);
        auto payload = context.consumeBytes(getLength(ensureExpectedTag(context, {0x7f, 0x21})));
        ensureEmpty(context);

        auto payloadContext = common::Context(payload);
        auto a = payloadContext.consumeBytes(getLength(ensureExpectedTag(payloadContext, {0x5f, 0x37})));
        auto b = payloadContext.consumeBytes(getLength(ensureExpectedTag(payloadContext, {0x5f, 0x38})));
        ensureEmpty(payloadContext);

        // TODO Find out what a and b is
        return {};
    }

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decode(Envelop const &envelop)
    {
        auto const publicKey = getPublicKey(envelop.certificate);

        auto const rootCertificate = certificateProvider.get("4555564456100106");
        auto const companyCertificate = certificateProvider.get(envelop.authority);

        /* The value in 'authority' should match exactly one very specific entry in
           the list of exported certificates from public LDAP server identified by
           'cn=<authority>,ou=VDV KA,o=VDV Kernapplikations GmbH,c=de'
           4555564456xxxxxx -> EUVDVxxxxxx
           4445564456xxxxxx -> DEVDVxxxxxx
        */

        if (companyCertificate && rootCertificate)
        {
            auto const rootKey = getRootKey(rootCertificate->certificate);
            auto const companyKey = getCompanyKey(companyCertificate->certificate);

            // auto rng = std::make_unique<Botan::AutoSeeded_RNG>();
            // auto dataSource = Botan::DataSource_Memory(rootCertificate->certificate.data(), rootCertificate->certificate.size());
            // auto const publicKey = Botan::X509_Certificate(dataSource);

            // auto decoder = Botan::PK_Decryptor_EME(*key, *rng, "RSA-1984(SHA-1)");
            // auto const message = decoder.decrypt(certificate.data(), certificate.size());
        }

        return std::nullopt;
    }
}
