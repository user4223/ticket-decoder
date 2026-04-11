// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/SBBInterpreter.h"

#include "lib/infrastructure/include/Logging.h"

#include <google/protobuf/util/json_util.h>
#include "sbb.pb.h"

#include <algorithm>

namespace interpreter::detail::sbb
{

    static std::vector<std::uint8_t> const typeId = {0x0A, 0xC8, 0x01}; // This is just 3 bytes of protobuf preable and not finally indicating it's an SBB ticket, but for now....

    SBBInterpreter::SBBInterpreter(infrastructure::LoggerFactory &lf, api::SignatureVerifier const &sc)
        : logger(CREATE_LOGGER(lf))
    {
    }

    bool SBBInterpreter::canInterpret(common::Context const &context) const
    {
        auto const head = context.peekMaximalBytes(typeId.size());
        return std::equal(std::begin(typeId), std::end(typeId), std::begin(head), std::end(head));
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
        auto json = std::string{};
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
