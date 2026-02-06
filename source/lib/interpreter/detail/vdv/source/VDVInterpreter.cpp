// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVInterpreter.h"
#include "../include/LDIFFileCertificateProvider.h"
#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

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
    ensureEmpty(context);

    auto const signatureIdent = common::bytesToString(signature.remainder.subspan(0, 3));
    auto const signatureVersion = common::bytesToHexString(signature.remainder.subspan(3, 2));

    auto jsonBuilder = utility::JsonBuilder::object();
    jsonBuilder
        .add("signatureIdent", signatureIdent)
        .add("signatureVersion", signatureVersion)
        .add("certificateAuthority", certificate.authority);

    auto messageContext = messageDecoder->decodeMessage(certificate, signature);
    if (messageContext)
    {
      auto const ticketId = common::consumeInteger4(*messageContext);
      jsonBuilder.add("ticketId", std::to_string(ticketId));
    }

    context.addRecord(common::Record(signatureIdent, signatureVersion, std::move(jsonBuilder)));
    return std::move(context);
  }
}
