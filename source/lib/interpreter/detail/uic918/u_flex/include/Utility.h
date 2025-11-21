#pragma once

#include <string>
#include <optional>

#include "Export.h"

namespace interpreter::detail::uic::u_flex
{
  TICKET_DECODER_EXPORT
  std::optional<std::string> daysAndYearToIsoDate(long const year, long const dayOfYear);

  TICKET_DECODER_EXPORT
  std::optional<std::string> daysAndYearToIsoDate(long const *const year, long const *const dayOfYear);

  TICKET_DECODER_EXPORT
  std::string minutesToIsoTime(long const noOfMinutes);

  TICKET_DECODER_EXPORT
  std::optional<std::string> minutesToIsoTime(long const *const noOfMinutes);

  TICKET_DECODER_EXPORT
  std::optional<std::string> quaterHoursToIsoZone(long const *const noOfQuaterHours);
}
