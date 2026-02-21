// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVInterpreter.h"
#include "../include/LDIFFileCertificateProvider.h"
#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/TLVDecoder.h"
#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/DateTimeDecoder.h"

#include "lib/utility/include/Base64.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::vdv
{

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

  common::Context VDVInterpreter::interpret(common::Context &&context)
  {
    // Documentation (not fully matching anymore):
    //  - https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf
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

    auto const remainderTail = common::Context(signature.remainder).consumeBytesEnd(5);
    auto const signatureIdent = common::StringDecoder::bytesToString(remainderTail.subspan(0, 3));
    auto const signatureVersion = common::StringDecoder::bytesToHexString(remainderTail.subspan(3, 2));

    auto jsonBuilder = utility::JsonBuilder::object();
    jsonBuilder
        .add("signatureIdent", signatureIdent)
        .add("signatureVersion", signatureVersion)
        .add("certificateAuthority", certificate.authority);

    auto message = messageDecoder->decodeMessage(certificate, signature);
    if (message)
    {
      auto messageContext = common::Context(*message);
      auto const messageTail = messageContext.consumeBytesEnd(5);
      auto const messageIdent = common::StringDecoder::bytesToString(messageTail.subspan(0, 3));
      auto const messageVersion = common::StringDecoder::bytesToHexString(messageTail.subspan(3, 2));
      jsonBuilder
          .add("ticketId", std::to_string(common::consumeInteger4(messageContext)))
          .add("ticketOrganisationId", std::to_string(common::consumeInteger2(messageContext)))
          .add("productNumber", std::to_string(common::consumeInteger2(messageContext)))
          .add("productOrganisationId", std::to_string(common::consumeInteger2(messageContext)))
          .add("validFrom", common::DateTimeDecoder::consumeDateTimeCompact4(messageContext))
          .add("validTo", common::DateTimeDecoder::consumeDateTimeCompact4(messageContext));

      auto const product = common::TLVDecoder::consumeExpectedElement(messageContext, {0x85});
      {
        auto productContext = common::Context(product);
        auto const unknown1 = common::TLVDecoder::consumeExpectedElement(productContext, {0xda});
        auto const passenger = common::TLVDecoder::consumeExpectedElement(productContext, {0xdb});
        {
          auto passengerContext = common::Context(passenger);
          auto const gender = std::to_string(passengerContext.consumeByte());
          auto const birthDate = common::DateTimeDecoder::consumeDateTimeCompact4(passengerContext);
          auto const name = common::StringDecoder::bytesToString(passengerContext.consumeRemainingBytes());
          jsonBuilder
              .add("name", name)
              .add("gender", gender);
        }
      }
    }

    context.addField("validated", message ? "true" : "false");

    context.addRecord(common::Record(signatureIdent, signatureVersion, std::move(jsonBuilder)));
    return std::move(context);
  }
}
