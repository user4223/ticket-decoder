#pragma once

#include <string>
#include <optional>

#include "Export.h"

namespace uic918::u_flex::utility
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
  std::optional<std::string> quaterHoursToZoneOffset(long const *const noOfQuaterHours);
}
