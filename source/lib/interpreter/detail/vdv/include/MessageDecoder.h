// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Certificate.h"

#include <optional>
#include <vector>
#include <cstdint>
#include <span>
#include <string>

namespace interpreter::detail::vdv
{
    class MessageDecoder
    {
    public:
        virtual ~MessageDecoder() = default;

        /* Takes certificate from envelope and signature and decodes the
           message by using root + issuing certificate internally.
         */
        virtual std::optional<std::vector<std::uint8_t>> decodeMessage(
            Certificate const &envelopeCertificate,
            Signature const &envelopeSignature) = 0;
    };
}
