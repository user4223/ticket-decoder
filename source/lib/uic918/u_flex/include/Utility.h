#pragma once

#include <string>
#include <optional>

#include "Export.h"

namespace uic918::u_flex::utility
{
  TICKET_DECODER_EXPORT
  std::optional<std::string> toIsoDate(long const *const year, long const *const dayOfYear);
}
