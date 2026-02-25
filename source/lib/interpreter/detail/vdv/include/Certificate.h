// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/detail/common/include/Context.h"

#include "lib/interpreter/api/include/CertificateProvider.h"

#include <span>
#include <cstdint>
#include <string>
#include <vector>

namespace interpreter::detail::vdv
{

    struct Signature
    {
        std::span<std::uint8_t const> value;
        std::span<std::uint8_t const> remainder;

        static Signature consumeFrom(common::Context &context);

        static Signature consumeFromEnvelope(common::Context &context);
    };

    struct PublicKey
    {
        std::span<std::uint8_t const> exponent;
        std::span<std::uint8_t const> modulus;

        static PublicKey consumeFrom(common::Context &context);
    };

    /* This object does not own the data, it's just a view.
     */
    struct Certificate
    {
        std::string authority;
        std::string description;
        Signature signature;

        static Certificate decodeFrom(api::CertificateRawData const &rawData);

        static Certificate consumeFromEnvelope(common::Context &context);
    };

    /* Has no fixed lengh, right now we are passing a length argument but this may break for any other ticket
       TODO Find a way to determine length automatically or to discover termination 2 avoid length argument!
     */
    struct CertificateOID
    {
        std::vector<std::string> parts;

        std::string toString() const;

        static CertificateOID consumeFrom(common::Context &context, std::size_t length);
    };

    /* 8 bytes long on wire
     */
    struct CertificateParticipant
    {
        std::string region;
        std::string name;
        int serviceIdenticator = 0;
        int algorithmReference = 0;
        std::string year;

        constexpr bool matches(CertificateParticipant const &other) const
        {
            return region == other.region                            //
                   && name == other.name                             //
                   && serviceIdenticator == other.serviceIdenticator //
                   && algorithmReference == other.algorithmReference //
                   && year == other.year;
        }

        std::string toString() const;

        static CertificateParticipant consumeFrom(common::Context &context);
    };

    /* 4 or 3 or 2 bytes long on wire
     */
    struct CertificateDate
    {
        std::uint16_t year = 0;
        std::uint8_t month = 0;
        std::uint8_t day = 0;

        std::string toString() const;

        static CertificateDate consumeFrom4(common::Context &context);

        static CertificateDate consumeFrom3(common::Context &context);

        static CertificateDate consumeFrom2(common::Context &context);
    };

    /* 12 bytes long on wire
     */
    struct CertificateReference
    {
        std::string organisationId;
        CertificateDate samValidUntil;
        CertificateDate samValidFrom;
        std::string ownerOrganisationId;
        std::string samId;

        std::string toString() const;

        static CertificateReference consumeFrom(common::Context &context);
    };

    /* 7 bytes long on wire
     */
    struct CertificateAuthorization
    {
        std::string name;
        int serviceIndicator = 0;

        std::string toString() const;

        static CertificateAuthorization consumeFrom(common::Context &context);
    };

    /* 1 byte long on wire
     */
    struct CertificateProfile
    {
        std::string identifier;

        std::string toString() const;

        static CertificateProfile consumeFrom(common::Context &context);
    };

    struct CertificateIdentity
    {
        CertificateProfile profile;
        CertificateParticipant authority;
        std::optional<CertificateParticipant> holder;
        std::optional<CertificateReference> reference;
        CertificateAuthorization authorization;
        CertificateDate expiryDate;
        CertificateOID algorithm;

        std::string toString() const;

        static CertificateIdentity consumeFrom(common::Context &context, std::size_t oidLength);
    };

    struct DecodedCertificate
    {
        std::optional<std::vector<std::uint8_t>> rawData;
        CertificateIdentity identity;
        PublicKey publicKey;

        static DecodedCertificate decodeRootFrom(api::CertificateRawData const &rawData);

        static DecodedCertificate decodeFrom(std::vector<std::uint8_t> &&content);
    };
}
