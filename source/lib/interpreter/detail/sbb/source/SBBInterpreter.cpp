// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/SBBInterpreter.h"

#include "lib/utility/include/Logging.h"

#include <google/protobuf/util/json_util.h>
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
        auto sbb = SBB();
        auto const bytes = context.consumeAllBytes();
        if (!sbb.ParsePartialFromArray(bytes.data(), bytes.size()))
        {
            LOG_WARN(logger) << "Failed to parse SBB protobuf message";
            // return context;
        }
        std::string json;
        auto const status = google::protobuf::util::MessageToJsonString(sbb, &json);
        if (!status.ok())
        {
            LOG_WARN(logger) << "Failed to convert SBB protobuf message into json";
            return context;
        }
        context.addRecord(common::Record("SBB", "", utility::JsonBuilder(json)));
        return context;
    }
}
