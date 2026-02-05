// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/infrastructure/include/Logging.h"

#include <botan/bigint.h>
#include <botan/numthry.h>
#include <botan/hash.h>

namespace interpreter::detail::vdv
{

    struct DecodedCertificate
    {
        std::optional<std::vector<std::uint8_t>> rawData;
        CertificateIdentity identity;
        PublicKey publicKey;

        static DecodedCertificate decodeRootFrom(std::span<std::uint8_t const> content)
        {
            auto context = common::Context(content);
            auto identity = CertificateIdentity::consumeFrom(context, 9);
            auto publicKey = PublicKey::consumeFrom(context);
            ensureEmpty(context);
            return DecodedCertificate{std::nullopt, std::move(identity), std::move(publicKey)};
        }

        static DecodedCertificate decodeFrom(std::vector<std::uint8_t> &&content)
        {
            auto context = common::Context(content);
            auto identity = CertificateIdentity::consumeFrom(context, 7); // TODO OID length is probably not always 7 for all sub-certificates
            auto publicKey = PublicKey::consumeFrom(context);
            ensureEmpty(context);
            return DecodedCertificate{std::make_optional(std::move(content)), std::move(identity), std::move(publicKey)};
        }
    };

    class BotanMessageDecoder::Internal
    {
        std::unique_ptr<Botan::HashFunction> const sha1HashFunction = Botan::HashFunction::create_or_throw("SHA-1");

    public:
        std::vector<std::uint8_t> decryptVerify(Signature const &signature, PublicKey const &publicKey)
        {
            auto contentContext = common::Context(Botan::power_mod(Botan::BigInt(signature.value),
                                                                   Botan::BigInt(publicKey.exponent),
                                                                   Botan::BigInt(publicKey.modulus))
                                                      .serialize());

            consumeExpectedFrameTags(contentContext, {0x6A}, {0xBC});
            auto const expectedHash = contentContext.consumeBytesEnd(20);
            auto content = contentContext.consumeRemainingBytesAppend(signature.remainder);
            ensureEmpty(contentContext);

            auto const actualHash = sha1HashFunction->process(content);
            if (!std::equal(actualHash.begin(), actualHash.end(), expectedHash.begin(), expectedHash.end()))
            {
                throw std::runtime_error("SHA1 hash value for content does not match expected hash value");
            }
            return content; // Copy of vector owning the data and moving out here
        }
    };

    BotanMessageDecoder::BotanMessageDecoder(infrastructure::LoggerFactory &lf, CertificateProvider &cp)
        : logger(CREATE_LOGGER(lf)),
          certificateProvider(cp),
          internal(std::make_shared<Internal>())
    {
    }

    std::optional<common::Context> BotanMessageDecoder::decodeMessage(
        Certificate const &envelopeCertificate,
        Signature const &envelopeSignature)
    {
        auto const rootCertificate = certificateProvider.getRoot();
        if (!rootCertificate)
        {
            LOG_INFO(logger) << "Root certificate not found";
            return std::nullopt;
        }
        auto const issuingCertificate = certificateProvider.get(envelopeCertificate.authority);
        if (!issuingCertificate)
        {
            LOG_INFO(logger) << "Issuing certificate not found: " << envelopeCertificate.authority;
            return std::nullopt;
        }

        auto const decodedRootCertificate = DecodedCertificate::decodeRootFrom(rootCertificate->content);

        LOG_INFO(logger) << "Using root certificate " << decodedRootCertificate.identity.toString();

        auto const decodedIssuingCertificate = DecodedCertificate::decodeFrom(
            internal->decryptVerify(issuingCertificate->signature, decodedRootCertificate.publicKey));

        LOG_INFO(logger) << "Using issuing certificate " << decodedIssuingCertificate.identity.toString();

        auto const decodedEnvelopeCertificate = DecodedCertificate::decodeFrom(
            internal->decryptVerify(envelopeCertificate.signature, decodedIssuingCertificate.publicKey));

        LOG_INFO(logger) << "Using envelope certificate " << decodedEnvelopeCertificate.identity.toString();

        return std::make_optional(common::Context(
            internal->decryptVerify(envelopeSignature, decodedEnvelopeCertificate.publicKey)));
    }
}
