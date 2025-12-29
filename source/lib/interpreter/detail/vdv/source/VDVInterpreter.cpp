// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVInterpreter.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::vdv
{

  static std::vector<std::uint8_t> const typeId = {0x9E, 0x81, 0x80};

  VDVInterpreter::TypeIdType VDVInterpreter::getTypeId()
  {
    return typeId;
  }

  VDVInterpreter::VDVInterpreter(infrastructure::LoggerFactory &lf, api::SignatureVerifier const &sc)
      : logger(CREATE_LOGGER(lf))
  {
  }

  common::Context VDVInterpreter::interpret(common::Context &&context)
  {
    // Documentation: https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf
    // Reference-Impl: https://sourceforge.net/projects/dbuic2vdvbc/
    auto const tag = common::getNumeric8(context);
    auto const signatureLength = common::getNumeric16(context);
    auto const signature = context.consumeBytes(128);

    auto const ticketDataTag = common::getNumeric8(context);
    auto const ticketDataLength = common::getNumeric8(context);
    auto const ticketData = context.consumeBytes(128);

    auto const keyTag = common::getNumeric16(context);
    auto const keyLength = common::getNumeric8(context);
    auto const key = context.consumeBytes(12);

    auto const ignored = context.ignoreRemainingBytes();
    LOG_WARN(logger) << "Unsupported VDV barcode detected of size: " << ignored;
    return std::move(context);
  }
}
