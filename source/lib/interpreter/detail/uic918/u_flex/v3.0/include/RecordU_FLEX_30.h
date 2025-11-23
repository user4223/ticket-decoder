// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/LoggingFwd.h"

#include <optional>
#include <vector>

#include "../../include/Export.h"

namespace interpreter::detail::uic::u_flex30
{
    TICKET_DECODER_EXPORT
    std::optional<::utility::JsonBuilder> convert(::utility::Logger &logger, std::vector<std::uint8_t> const &bytes);
}
