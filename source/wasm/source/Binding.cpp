// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <emscripten/bind.h>

#include "lib/infrastructure/include/Context.h"

#include "lib/api/include/DecoderFacade.h"

class DecoderFacadeWrapper
{
    std::string decodeBase64(std::string const &base64)
    {
        return "hello world";
    }
};

EMSCRIPTEN_BINDINGS(TicketDecoder) {

    emscripten::class_<DecoderFacadeWrapper>("DecoderFacade")
        .constructor<>()
        .function("decodeBase64", &DecoderFacadeWrapper::decodeBase64);
};
