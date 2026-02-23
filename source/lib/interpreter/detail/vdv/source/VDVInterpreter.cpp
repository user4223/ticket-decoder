// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVInterpreter.h"
#include "../include/LDIFFileCertificateProvider.h"
#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/NumberDecoder.h"
#include "lib/interpreter/detail/common/include/TLVDecoder.h"
#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/DateTimeDecoder.h"
#include "lib/interpreter/detail/common/include/BCDDecoder.h"

#include "lib/utility/include/Base64.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::vdv
{
  using namespace common;

  /* This is actually not a fixed ident. 0x9e is a BER-TLV tag (signature) and 0x81+0x80 a length (128 bytes).
     But it should be sufficient for now to identify VDV tickets.
  */
  static std::vector<std::uint8_t> const typeId = {0x9E, 0x81, 0x80};

  VDVInterpreter::TypeIdType VDVInterpreter::getTypeId()
  {
    return typeId;
  }

  VDVInterpreter::VDVInterpreter(infrastructure::LoggerFactory &lf)
      : logger(CREATE_LOGGER(lf)),
        certificateProvider(std::make_unique<LDIFFileCertificateProvider>(lf)),
        messageDecoder(std::make_unique<BotanMessageDecoder>(lf, *certificateProvider))
  {
  }

  static void decodePrimaryData(std::span<std::uint8_t const> bytes, utility::JsonBuilder &jsonResult)
  {
    auto context = Context(bytes);
    context.ignoreBytes(7);
    auto const price = NumberDecoder::consumeInteger4(context);
    jsonResult
        .add("price", price);
  }

  static void decodePassengerData(std::span<std::uint8_t const> bytes, utility::JsonBuilder &jsonResult)
  {
    auto context = Context(bytes);
    auto const gender = std::to_string(context.consumeByte());
    auto const dateOfBirth = DateTimeDecoder::consumeDateTimeCompact4(context);
    auto const name = StringDecoder::decodeLatin1(context.consumeRemainingBytes());
    jsonResult
        .add("gender", gender)
        .add("name", name)
        .add("dateOfBirth", dateOfBirth);
  }

  static void decodeIdentificationData(std::span<std::uint8_t const> bytes, utility::JsonBuilder &jsonResult)
  {
  }

  common::Context VDVInterpreter::interpret(common::Context &&context)
  {
    // Documentation (not fully matching anymore):
    //  - https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf
    //  - https://www.kcd-nrw.de/fileadmin/user_upload/Abbildung_und_Kontrolle_in_NRW_1_5_5.pdf
    // Quite old reference impl for encoding:
    //  - https://sourceforge.net/projects/dbuic2vdvbc/
    // Some more up-to-date hints:
    //  - https://magicalcodewit.ch/38c3-slides/#/32
    //  - https://github.com/TheEnbyperor/zuegli/tree/root/main/vdv
    //  - https://github.com/akorb/deutschlandticket_parser/blob/main/main.py
    //  - https://github.com/RWTH-i5-IDSG/ticketserver/blob/master/barti-check/src/main/java/de/rwth/idsg/barti/check/Decode.java

    auto const signature = Signature::consumeFromEnvelope(context);
    auto const certificate = Certificate::consumeFromEnvelope(context);
    context.ensureEmpty();

    auto const remainderTail = Context(signature.remainder).consumeBytesEnd(5);
    auto const signatureIdent = StringDecoder::decodeLatin1(remainderTail.subspan(0, 3));
    auto const signatureVersion = std::to_string(BCDDecoder::decodePackedInteger2(remainderTail.subspan(3, 2)));

    auto jsonBuilder = utility::JsonBuilder::object();
    jsonBuilder
        .add("signatureIdent", signatureIdent)
        .add("signatureVersion", signatureVersion)
        .add("certificateAuthority", certificate.authority);

    auto message = messageDecoder->decodeMessage(certificate, signature);
    if (message)
    {
      auto messageContext = Context(*message);
      auto const messageTail = messageContext.consumeBytesEnd(5);
      auto const messageIdent = StringDecoder::decodeLatin1(messageTail.subspan(0, 3));
      auto const messageVersion = BCDDecoder::decodePackedInteger2(messageTail.subspan(3, 2));
      jsonBuilder
          .add("ticketId", std::to_string(NumberDecoder::consumeInteger4(messageContext)))
          .add("ticketOrganisationId", std::to_string(NumberDecoder::consumeInteger2(messageContext)))
          .add("productNumber", std::to_string(NumberDecoder::consumeInteger2(messageContext)))
          .add("productOrganisationId", std::to_string(NumberDecoder::consumeInteger2(messageContext)))
          .add("validFrom", DateTimeDecoder::consumeDateTimeCompact4(messageContext))
          .add("validTo", DateTimeDecoder::consumeDateTimeCompact4(messageContext));

      auto const efsDecoder = TLVDecoder({// clang-format off
          {{0xDA}, [&](auto bytes) { decodePrimaryData(std::move(bytes), jsonBuilder); }},
          {{0xDB}, [&](auto bytes) { decodePassengerData(std::move(bytes), jsonBuilder); }},
          {{0xD7}, [&](auto bytes) { decodeIdentificationData(std::move(bytes), jsonBuilder); }}
        }); // clang-format on
      efsDecoder.consume(TLVDecoder::consumeExpectedElement(messageContext, {0x85}));
    }

    context.addField("validated", message ? "true" : "false");

    context.addRecord(Record(signatureIdent, signatureVersion, std::move(jsonBuilder)));
    return std::move(context);
  }
}
