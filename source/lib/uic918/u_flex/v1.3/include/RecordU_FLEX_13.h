
#pragma once

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/LoggingFwd.h"

#include <optional>
#include <vector>

namespace uic918::detail
{
    std::optional<::utility::JsonBuilder> convertV13(::utility::Logger &logger, std::vector<std::uint8_t> const &bytes);
}
