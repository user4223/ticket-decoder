// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVInterpreter.h"
#include "../include/LDIFFileCertificateProvider.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/infrastructure/include/Logging.h"

#include <array>

namespace interpreter::detail::vdv
{

  static std::vector<std::uint8_t> const typeId = {0x9E, 0x81, 0x80};

  VDVInterpreter::TypeIdType VDVInterpreter::getTypeId()
  {
    return typeId;
  }

  /* Multi-byte integers are big-endian encoded, see DER ASN.1
   */
  std::uint32_t getLength(common::Context &context)
  {
    auto const first = common::getNumeric8(context);
    // clang-format off
    if (first < 0x80) { return first; }
    else if (first == 0x81) { return common::getNumeric8(context); }
    else if (first == 0x82) { return common::getNumeric16(context); }
    else if (first == 0x83) { return common::getNumeric24(context); }
    else if (first == 0x84) { return common::getNumeric32(context); }
    // clang-format on
    throw std::runtime_error(std::string("Found unexpected length indicator tag (expecting x<0x80 or x=0x8y with y=<remaining bytes>): ") + std::to_string(first));
  }

  using TagType = std::array<std::uint8_t, 2>;

  TagType getTag(common::Context &context)
  {
    auto const first = common::getNumeric8(context);
    if (first == 0x7f || first == 0x5f)
    {
      return {first, common::getNumeric8(context)};
    }

    return {first, 0};
  }

  VDVInterpreter::VDVInterpreter(infrastructure::LoggerFactory &lf, api::SignatureVerifier const &sc)
      : logger(CREATE_LOGGER(lf)), certificateProvider(std::make_unique<LDIFFileCertificateProvider>(lf))
  {
  }

  common::Context VDVInterpreter::interpret(common::Context &&context)
  {
    // Documentation (not fully matching anymore):
    //  - https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf
    // Quite old reference impl for encoding:
    //  - https://sourceforge.net/projects/dbuic2vdvbc/
    // Some more up-to-date hints:
    //  - https://magicalcodewit.ch/38c3-slides/#/32
    //  - https://github.com/akorb/deutschlandticket_parser/blob/main/main.py
    //  - https://github.com/RWTH-i5-IDSG/ticketserver/blob/master/barti-check/src/main/java/de/rwth/idsg/barti/check/Decode.java

    auto const signatureTag = getTag(context);
    if (signatureTag != TagType{0x9e, 0})
    {
      throw std::runtime_error(std::string("Unexpected signature tag found: 0x") + common::bytesToString(signatureTag));
    }
    auto const signatureLength = getLength(context);
    if (signatureLength != 128)
    {
      throw std::runtime_error(std::string("Unexpected signature length found: ") + std::to_string(signatureLength));
    }
    auto const signature = context.consumeBytes(signatureLength);

    auto const ticketIdentTag = getTag(context);
    if (ticketIdentTag != TagType{0x9a, 0})
    {
      throw std::runtime_error(std::string("Unexpected ticket ident tag found: 0x") + common::bytesToString(ticketIdentTag));
    }
    auto const ticketIdentLength = getLength(context);
    if (ticketIdentLength != 5)
    {
      throw std::runtime_error(std::string("Unexpected ticket ident length found: ") + std::to_string(ticketIdentLength));
    }
    auto const ticketIdent = common::getAlphanumeric(context, 3);
    if (ticketIdent != std::string("VDV"))
    {
      throw std::runtime_error(std::string("Unexpected ticket ident found: ") + ticketIdent);
    }
    auto const ticketVersion = common::bytesToString(context.consumeBytes(2));
    // TODO Handle version somehow

    auto const certificateTag = getTag(context);
    if (certificateTag != TagType{0x7f, 0x21})
    {
      throw std::runtime_error(std::string("Unexpected certificate tag found: 0x") + common::bytesToString(certificateTag));
    }
    auto const certificateLength = getLength(context);
    if (certificateLength != 200)
    {
      throw std::runtime_error(std::string("Unexpected certificate length found: ") + std::to_string(certificateLength));
    }
    auto const certificate = context.consumeBytes(certificateLength);

    auto const authorityTag = getTag(context);
    if (authorityTag != TagType{0x42, 0})
    {
      throw std::runtime_error(std::string("Unexpected authority tag found: 0x") + common::bytesToString(authorityTag));
    }
    auto const authorityLength = getLength(context);
    if (authorityLength != 8)
    {
      throw std::runtime_error(std::string("Unexpected authority length found: ") + std::to_string(authorityLength));
    }
    auto const authority = common::bytesToString(context.consumeBytes(authorityLength));

    /* The value in 'authority' should match exactly one very specific entry in
       the list of exported certificates from public LDAP server identified by
       'cn=<authority>,ou=VDV KA,o=VDV Kernapplikations GmbH,c=de'
       4555564456xxxxxx -> EUVDVxxxxxx
       4445564456xxxxxx -> DEVDVxxxxxx
    */
    auto const companyCertificate = certificateProvider->get(authority);
    auto const rootCertificate = certificateProvider->get("4555564456100106");

    return std::move(context);
  }
}
