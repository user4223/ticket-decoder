
#pragma once

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/LoggingFwd.h"

#include <optional>
#include <vector>

#include "../../include/Export.h"

namespace uic918::u_flex30
{
    TICKET_DECODER_EXPORT
    std::optional<::utility::JsonBuilder> convert(::utility::Logger &logger, std::vector<std::uint8_t> const &bytes);
}
