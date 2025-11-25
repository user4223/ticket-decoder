// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/SBBInterpreter.h"

#include "lib/utility/include/Logging.h"

#include "sbb.pb.h"

namespace interpreter::detail::sbb
{
    SBBInterpreter::TypeIdType SBBInterpreter::getTypeId()
    {
        return {0x0A, 0xC8, 0x01}; // This is just 3 bytes of protobuf preable and not finally indicating it's an SBB ticket, but for now....
    }

    SBBInterpreter::SBBInterpreter(::utility::LoggerFactory &lf)
        : logger(CREATE_LOGGER(lf))
    {
    }

    common::Context SBBInterpreter::interpret(common::Context &&context)
    {
        LOG_WARN(logger) << "Unsupported SBB barcode detected";
        return std::move(context);
    }
}
