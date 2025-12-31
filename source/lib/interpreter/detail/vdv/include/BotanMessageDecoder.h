// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "MessageDecoder.h"
#include "CertificateProvider.h"

#include "lib/infrastructure/include/Logger.h"

namespace interpreter::detail::vdv
{
    class BotanMessageDecoder : public MessageDecoder
    {
        infrastructure::Logger logger;
        CertificateProvider &certificateProvider;

    public:
        BotanMessageDecoder(infrastructure::LoggerFactory &loggerFactory, CertificateProvider &certificateProvider);

        virtual std::optional<std::vector<std::uint8_t>> decode(Envelop const &envelop) override;
    };
}
