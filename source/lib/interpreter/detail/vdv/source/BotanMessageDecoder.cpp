// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/NumberDecoder.h"

#include "lib/infrastructure/include/Logging.h"

#include <botan/bigint.h>
#include <botan/numthry.h>
#include <botan/hash.h>

namespace interpreter::detail::vdv
{

    class BotanMessageDecoder::Internal
    {
        std::unique_ptr<Botan::HashFunction> const sha1HashFunction = Botan::HashFunction::create_or_throw("SHA-1");

        CertificateProvider &certificateProvider;
        std::optional<DecodedCertificate> rootCertificate;

    public:
        Internal(CertificateProvider &cp)
            : certificateProvider(cp),
              rootCertificate()
        {
            auto const certificate = certificateProvider.get("4555564456100106"); // EUVDV, 16, 01, 1996 - self signed root certificate
            rootCertificate = certificate
                                  ? std::make_optional(DecodedCertificate::decodeRootFrom(certificate->content))
                                  : std::nullopt;
        }

        bool isEnabled() const
        {
            return rootCertificate.has_value();
        }

        std::optional<DecodedCertificate> decryptIssuingCertificate(std::string authority)
        {
            if (!isEnabled())
            {
                return std::nullopt;
            }

            auto const certificate = certificateProvider.get(authority);
            return certificate
                       ? std::make_optional(DecodedCertificate::decodeFrom(decryptVerify(certificate->signature, rootCertificate->publicKey)))
                       : std::nullopt;
        }

        std::vector<std::uint8_t> decryptVerify(Signature const &signature, PublicKey const &publicKey)
        {
            auto context = common::Context(Botan::power_mod(Botan::BigInt(signature.value),
                                                            Botan::BigInt(publicKey.exponent),
                                                            Botan::BigInt(publicKey.modulus))
                                               .serialize());

            auto const head = context.consumeByte();
            auto const tail = context.consumeByteEnd();
            if (head != 0x6A || tail != 0xBC)
            {
                throw std::runtime_error(std::string("Expected head 0x6A / tail 0xBC bytes not found") + std::to_string(head) + "/" + std::to_string(tail));
            }
            auto const expectedHash = context.consumeBytesEnd(20);
            auto content = context.consumeRemainingBytesAppend(signature.remainder);
            context.ensureEmpty();

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
          internal(std::make_shared<Internal>(cp)),
          issuingCertificates()
    {
        if (!internal->isEnabled())
        {
            LOG_INFO(logger) << "Decryption disabled, certificates not found or unable to read";
        }
    }

    std::optional<DecodedCertificate> BotanMessageDecoder::getIssuingCertificate(std::string authority)
    {
        auto cacheEntry = issuingCertificates.find(authority);
        if (cacheEntry != issuingCertificates.end())
        {
            return cacheEntry->second;
        }

        auto certificate = internal->decryptIssuingCertificate(authority);
        return issuingCertificates.emplace(std::make_pair(authority, std::move(certificate))).first->second;
    }

    std::optional<std::vector<std::uint8_t>> BotanMessageDecoder::decodeMessage(
        Certificate const &certificate,
        Signature const &signature)
    {
        if (!internal->isEnabled())
        {
            return std::nullopt;
        }

        /* TODO Verify issuer and holder identiy for the entire chain of certificates
         */

        auto const issuingCertificate = getIssuingCertificate(certificate.authority);
        if (!issuingCertificate)
        {
            LOG_INFO(logger) << "Decryption faild, issuing certificate not found: " << certificate.authority;
            return std::nullopt;
        }

        auto const envelopeCertificate = DecodedCertificate::decodeFrom(
            internal->decryptVerify(certificate.signature, issuingCertificate->publicKey));

        LOG_DEBUG(logger) << "Using envelope certificate " << envelopeCertificate.identity.toString();

        return std::make_optional(
            internal->decryptVerify(signature, envelopeCertificate.publicKey));
    }
}
