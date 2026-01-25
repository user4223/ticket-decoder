// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Certificate.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include <sstream>
#include <numeric>

#include <boost/algorithm/string.hpp>

namespace interpreter::detail::vdv
{

  Signature Signature::consumeFrom(common::Context &context)
  {
    auto const value = consumeExpectedTagValue(context, {0x5f, 0x37});
    auto const remainder = consumeExpectedTagValue(context, {0x5f, 0x38});
    return Signature{value, remainder};
  }

  Signature Signature::consumeFromEnvelope(common::Context &context)
  {
    auto const value = consumeExpectedTagValue(context, {0x9e, 0x00});
    auto const remainder = consumeExpectedTagValue(context, {0x9a, 0x00});
    return Signature{value, remainder};
  }

  PublicKey PublicKey::consumeFrom(common::Context &context)
  {                                                   // 65537 = 2^16+1 is the fifth Fermat, used commonly as exponent in RSA algorithms
    auto const exponent = context.consumeBytesEnd(4); // TODO Length unsure, always 4 bytes for exponent?
    auto const modulus = context.consumeRemainingBytes();
    return PublicKey{exponent, modulus};
  }

  Certificate Certificate::consumeFromEnvelope(common::Context &context)
  {
    auto const signatureData = consumeExpectedTagValue(context, {0x7f, 0x21});
    auto const authority = common::bytesToHexString(consumeExpectedTagValue(context, {0x42, 0x00}));

    auto signatureContext = common::Context(signatureData);
    auto signature = Signature::consumeFrom(signatureContext);
    ensureEmpty(signatureContext);

    return Certificate{authority, "envelope", signature, {}};
  }

  CertificateOID CertificateOID::consumeFrom(common::Context &inputContext, std::size_t length)
  {
    /* There is no 0x06 tag in the beginning and there is also no byte-length encoded.
       So it should be a fixed length or there is a termination indicator, but
       the specification does not mention a terminator.
    */
    auto context = common::Context(inputContext.consumeBytes(length));
    auto parts = std::vector<std::uint32_t>{};

    auto const header = common::getNumeric8(context);
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
      for (chunk = common::getNumeric8(context); chunk & 0x80; chunk = common::getNumeric8(context))
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
    auto const region = common::bytesToAlphanumeric(context.consumeBytes(2));
    auto const name = common::bytesToAlphanumeric(context.consumeBytes(3));
    auto const serviceIdenticator = common::getNumeric8(context);
    auto const algorithmReference = common::getNumeric8(context);
    auto const year = std::to_string(1990 + common::getNumeric8(context));
    return CertificateParticipant{region, name, serviceIdenticator, algorithmReference, year};
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
    auto const orgId = std::to_string(common::getNumeric16(context));
    auto const samValidUntil = CertificateDate::consumeFrom2(context);
    auto const samValidFrom = CertificateDate::consumeFrom3(context);
    auto const ownerOrgId = std::to_string(common::getNumeric16(context));
    auto const samId = std::to_string(common::getNumeric24(context));
    return CertificateReference{orgId, samValidUntil, samValidFrom, ownerOrgId, samId};
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
    auto const year = common::getDecimal16(context);
    auto const month = common::getDecimal8(context);
    auto const day = common::getDecimal8(context);
    return CertificateDate{year, month, day};
  }

  CertificateDate CertificateDate::consumeFrom3(common::Context &context)
  {
    auto const year = static_cast<std::uint16_t>(2000 + common::getDecimal8(context));
    auto const month = common::getDecimal8(context);
    auto const day = common::getDecimal8(context);
    return CertificateDate{year, month, day};
  }

  CertificateDate CertificateDate::consumeFrom2(common::Context &context)
  {
    auto const year = static_cast<std::uint16_t>(2000 + common::getDecimal8(context));
    auto const month = common::getDecimal8(context);
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
    auto const name = common::getAlphanumeric(context, 6);
    auto const serviceIndicator = common::getNumeric8(context);
    return CertificateAuthorization{name, serviceIndicator};
  }

  std::string CertificateProfile::toString() const
  {
    return identifier;
  }

  CertificateProfile CertificateProfile::consumeFrom(common::Context &context)
  {
    auto const identifier = std::to_string(common::getNumeric8(context));
    return CertificateProfile{identifier};
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
    auto const profile = CertificateProfile::consumeFrom(context);
    auto const authority = CertificateParticipant::consumeFrom(context);
    auto holder = std::optional<CertificateParticipant>{};
    auto reference = std::optional<CertificateReference>{};
    if (peekExpected(context, {0, 0, 0, 0}))
    {
      auto const fillBytes = context.consumeBytes(4);
      holder = std::make_optional(CertificateParticipant::consumeFrom(context));
    }
    else
    {
      reference = std::make_optional(CertificateReference::consumeFrom(context));
    }
    auto const authorization = CertificateAuthorization::consumeFrom(context);
    auto const expiryDate = CertificateDate::consumeFrom4(context);
    auto const algorithm = CertificateOID::consumeFrom(context, oidLength);
    return CertificateIdentity{profile, authority, holder, reference, authorization, expiryDate, algorithm};
  }
}
