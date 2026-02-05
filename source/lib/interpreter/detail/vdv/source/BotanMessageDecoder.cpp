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

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decodeMessage(
        Certificate const &envelopeCertificate,
        Signature const &envelopeSignature)
    {
        auto const rootCertificate = certificateProvider.getRoot();
        if (!rootCertificate)
        {
            return std::nullopt;
        }
        auto const issuingCertificate = certificateProvider.get(envelopeCertificate.authority);
        if (!issuingCertificate)
        {
            return std::nullopt;
        }

        auto rootContext = common::Context(rootCertificate->content);
        auto const rootCertIdentity = CertificateIdentity::consumeFrom(rootContext, 9);
        auto const rootCertPublicKey = PublicKey::consumeFrom(rootContext);
        ensureEmpty(rootContext);

        LOG_INFO(logger) << "Using root certificate " << rootCertIdentity.toString();

        auto issuingContext = common::Context(internal->decryptVerify(issuingCertificate->signature, rootCertPublicKey));
        auto const issuingCertIdentity = CertificateIdentity::consumeFrom(issuingContext, 7);
        auto const issuingCertPublicKey = PublicKey::consumeFrom(issuingContext);
        ensureEmpty(issuingContext);

        LOG_INFO(logger) << "Using issuing certificate " << issuingCertIdentity.toString();

        auto envelopeContext = common::Context(internal->decryptVerify(envelopeCertificate.signature, issuingCertPublicKey));
        auto const envelopeCertIdentity = CertificateIdentity::consumeFrom(envelopeContext, 7);
        auto const envelopeCertPublicKey = PublicKey::consumeFrom(envelopeContext);
        ensureEmpty(envelopeContext);

        LOG_INFO(logger) << "Using envelope certificate " << envelopeCertIdentity.toString();

        return std::make_optional(internal->decryptVerify(envelopeSignature, envelopeCertPublicKey));
    }
}
