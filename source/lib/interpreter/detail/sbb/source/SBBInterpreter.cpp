// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/SBBInterpreter.h"

#include "lib/utility/include/Logging.h"

#include <google/protobuf/util/json_util.h>
#include "sbb.pb.h"

namespace interpreter::detail::sbb
{

    static std::vector<std::uint8_t> const typeId = {0x0A, 0xC8, 0x01}; // This is just 3 bytes of protobuf preable and not finally indicating it's an SBB ticket, but for now....

    SBBInterpreter::TypeIdType SBBInterpreter::getTypeId()
    {
        return typeId;
    }

    SBBInterpreter::SBBInterpreter(::utility::LoggerFactory &lf)
        : logger(CREATE_LOGGER(lf))
    {
    }

    common::Context SBBInterpreter::interpret(common::Context &&context)
    {
        auto sbb = SBB();
        auto const bytes = context.consumeRemainingBytes();
        if (!sbb.ParsePartialFromArray(bytes.data(), bytes.size()))
        {
            LOG_WARN(logger) << "Failed to parse SBB protobuf message, trying to continue...";
            // return context;
        }
        std::string json;
        auto const status = google::protobuf::util::MessageToJsonString(sbb, &json);
        if (!status.ok())
        {
            LOG_WARN(logger) << "Failed to convert SBB protobuf message into json: " << status;
            return context;
        }

        context.addRecord(common::Record("SBB", "", utility::JsonBuilder(json)));
        context.addField("raw", context.getAllBase64Encoded());
        context.addField("validated", "false");
        return context;
    }
}
