// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Certificate.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/TLVDecoder.h"
#include "lib/interpreter/detail/common/include/BCDDecoder.h"
#include "lib/interpreter/detail/common/include/StringDecoder.h"

#include <sstream>
#include <numeric>

#include <boost/algorithm/string.hpp>

namespace interpreter::detail::vdv
{

  Signature Signature::consumeFrom(common::Context &context)
  {
    auto value = common::TLVDecoder::consumeExpectedElement(context, {0x5f, 0x37});
    auto remainder = common::TLVDecoder::consumeExpectedElement(context, {0x5f, 0x38});
    return Signature{std::move(value), std::move(remainder)};
  }

  Signature Signature::consumeFromEnvelope(common::Context &context)
  {
    auto value = common::TLVDecoder::consumeExpectedElement(context, {0x9e});
    auto remainder = common::TLVDecoder::consumeExpectedElement(context, {0x9a});
    return Signature{std::move(value), std::move(remainder)};
  }

  PublicKey PublicKey::consumeFrom(common::Context &context)
  {                                                   // 65537 = 2^16+1 is the fifth Fermat, used commonly as exponent in RSA algorithms
    auto exponent = context.consumeBytesEnd(4);       // TODO Length unsure, always 4 bytes for exponent?
    auto modulus = context.consumeRemainingBytes();
    return PublicKey{std::move(exponent), std::move(modulus)};
  }

  Certificate Certificate::consumeFromEnvelope(common::Context &context)
  {
    auto const signatureData = common::TLVDecoder::consumeExpectedElement(context, {0x7f, 0x21});
    auto authority = common::StringDecoder::bytesToHexString(common::TLVDecoder::consumeExpectedElement(context, {0x42}));

    auto signatureContext = common::Context(signatureData);
    auto signature = Signature::consumeFrom(signatureContext);
    context.ensureEmpty();

    return Certificate{std::move(authority), "envelope", std::move(signature), {}};
  }

  CertificateOID CertificateOID::consumeFrom(common::Context &inputContext, std::size_t length)
  {
    /* There is no 0x06 tag in the beginning and there is also no byte-length encoded.
       So it should be a fixed length or there is a termination indicator, but
       the specification does not mention a terminator.
    */
    auto context = common::Context(inputContext.consumeBytes(length));
    auto parts = std::vector<std::uint32_t>{};

    auto const header = common::consumeInteger1(context);
    if (header < 40) {          // ITU-T
      parts.insert(parts.begin(), {0, header});
    } else if (header < 80) {   // ISO
      parts.insert(parts.begin(), {1, header - 40u});
    } else {                    // joint-iso-itu-t
      parts.insert(parts.begin(), {2, header - 80u});
    }

    for (;length > 1 && !context.isEmpty(); length -= 1u)
    {
      auto part = std::uint32_t{0};
      auto chunk = std::uint32_t{0}; //            MSB = 1 means more bytes
      for (chunk = common::consumeInteger1(context); chunk & 0x80; chunk = common::consumeInteger1(context))
      {
        part = (part | (chunk & 0x7f)) << 7; // Drop MSB, OR it to what we have already and shift left to ensure space 4 next chunk
      }

      parts.push_back(part | chunk);
    }

    return CertificateOID{std::accumulate(std::begin(parts), std::end(parts), std::vector<std::string>{}, [](auto &&result, auto value)
                                          {
      result.emplace_back(std::to_string(value));
      return std::move(result); })};
  }

  std::string CertificateOID::toString() const
  {
    return boost::algorithm::join(parts, ".");
  }

  CertificateParticipant CertificateParticipant::consumeFrom(common::Context &context)
  {
    auto region = common::StringDecoder::decodeLatin1(context.consumeBytes(2));
    auto name = common::StringDecoder::decodeLatin1(context.consumeBytes(3));
    auto serviceIdenticator = common::consumeInteger1(context);
    auto algorithmReference = common::consumeInteger1(context);
    auto year = std::to_string(1990 + common::consumeInteger1(context));
    return CertificateParticipant{std::move(region), std::move(name), serviceIdenticator, algorithmReference, std::move(year)};
  }

  std::string CertificateParticipant::toString() const
  {
    auto out = std::ostringstream();
    out << region << "-"
        << name << "-"
        << year << "-"
        << "SVC(" << serviceIdenticator << ")-"
        << "ALG(" << algorithmReference << ")";
    return out.str();
  }

  std::string CertificateReference::toString() const
  {
    auto out = std::ostringstream();
    out << organisationId << "/"
        << ownerOrganisationId << "-"
        << "SAM(" << samId << ")-"
        << "FROM(" << samValidFrom.toString() << ")-"
        << "UNTIL(" << samValidUntil.toString() << ")";
    return out.str();
  }

  CertificateReference CertificateReference::consumeFrom(common::Context &context)
  {
    auto orgId = std::to_string(common::consumeInteger2(context));
    auto samValidUntil = CertificateDate::consumeFrom2(context);
    auto samValidFrom = CertificateDate::consumeFrom3(context);
    auto ownerOrgId = std::to_string(common::consumeInteger2(context));
    auto samId = std::to_string(common::consumeInteger3(context));
    return CertificateReference{std::move(orgId), std::move(samValidUntil), std::move(samValidFrom), std::move(ownerOrgId), std::move(samId)};
  }

  std::string CertificateDate::toString() const
  {
    auto out = std::ostringstream();
    out << std::setw(2) << std::setfill('0') << (int)day << "-"
        << std::setw(2) << std::setfill('0') << (int)month << "-"
        << std::setw(4) << std::setfill('0') << (int)year;
    return out.str();
  }

  CertificateDate CertificateDate::consumeFrom4(common::Context &context)
  {
    auto const year = common::BCDDecoder::consumePackedInteger2(context);
    auto const month = common::BCDDecoder::consumePackedInteger1(context);
    auto const day = common::BCDDecoder::consumePackedInteger1(context);
    return CertificateDate{year, month, day};
  }

  CertificateDate CertificateDate::consumeFrom3(common::Context &context)
  {
    auto const year = static_cast<std::uint16_t>(2000 + common::BCDDecoder::consumePackedInteger1(context));
    auto const month = common::BCDDecoder::consumePackedInteger1(context);
    auto const day = common::BCDDecoder::consumePackedInteger1(context);
    return CertificateDate{year, month, day};
  }

  CertificateDate CertificateDate::consumeFrom2(common::Context &context)
  {
    auto const year = static_cast<std::uint16_t>(2000 + common::BCDDecoder::consumePackedInteger1(context));
    auto const month = common::BCDDecoder::consumePackedInteger1(context);
    return CertificateDate{year, month, 1};
  }

  std::string CertificateAuthorization::toString() const
  {
    auto out = std::ostringstream();
    out << name << "-"
        << "SVC(" << serviceIndicator << ")";
    return out.str();
  }

  CertificateAuthorization CertificateAuthorization::consumeFrom(common::Context &context)
  {
    auto name = common::StringDecoder::consumeUTF8(context, 6);
    auto const serviceIndicator = common::consumeInteger1(context);
    return CertificateAuthorization{std::move(name), serviceIndicator};
  }

  std::string CertificateProfile::toString() const
  {
    return identifier;
  }

  CertificateProfile CertificateProfile::consumeFrom(common::Context &context)
  {
    auto identifier = std::to_string(common::consumeInteger1(context));
    return CertificateProfile{std::move(identifier)};
  }

  std::string CertificateIdentity::toString() const
  {
    auto out = std::ostringstream();
    out << "PROFILE(" << profile.toString() << ")-"
        << "AUTHORITY(" << authority.toString() << ")-";
    if (holder)
    {
      out << "HOLDER(" << holder->toString() << ")-";
    }
    if (reference)
    {
      out << "REFERENCE(" << reference->toString() << ")-";
    }
    out << "AUTHORIZATION(" << authorization.toString() << ")-"
        << "EXPIRY(" << expiryDate.toString() << ")-"
        << "ALG(" << algorithm.toString() << ")";
    return out.str();
  }

  CertificateIdentity CertificateIdentity::consumeFrom(common::Context &context, std::size_t oidLength)
  {
    auto profile = CertificateProfile::consumeFrom(context);
    auto authority = CertificateParticipant::consumeFrom(context);
    auto holder = std::optional<CertificateParticipant>{};
    auto reference = std::optional<CertificateReference>{};
    if (context.ignoreBytesIf({0, 0, 0, 0}))
    {
      holder = std::make_optional(CertificateParticipant::consumeFrom(context));
    }
    else
    {
      reference = std::make_optional(CertificateReference::consumeFrom(context));
    }
    auto authorization = CertificateAuthorization::consumeFrom(context);
    auto expiryDate = CertificateDate::consumeFrom4(context);
    auto algorithm = CertificateOID::consumeFrom(context, oidLength);
    return CertificateIdentity{std::move(profile), std::move(authority), std::move(holder), std::move(reference), std::move(authorization), std::move(expiryDate), std::move(algorithm)};
  }

  DecodedCertificate DecodedCertificate::decodeRootFrom(std::span<std::uint8_t const> content)
  {
    auto context = common::Context(content);
    auto identity = CertificateIdentity::consumeFrom(context, 9);
    auto publicKey = PublicKey::consumeFrom(context);
    context.ensureEmpty();
    return DecodedCertificate{std::nullopt, std::move(identity), std::move(publicKey)};
  }

  DecodedCertificate DecodedCertificate::decodeFrom(std::vector<std::uint8_t> &&content)
  {
    auto context = common::Context(content);
    auto identity = CertificateIdentity::consumeFrom(context, 7); // TODO OID length is probably not always 7 for all sub-certificates
    auto publicKey = PublicKey::consumeFrom(context);
    context.ensureEmpty();
    return DecodedCertificate{std::make_optional(std::move(content)), std::move(identity), std::move(publicKey)};
  }
}
