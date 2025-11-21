
#include "../include/UflexUtility.h"

#include <array>
#include <iomanip>
#include <sstream>
#include <algorithm>
// #if _LIBCPP_STD_VER >= 20
// #include <format>
// #endif
#include <chrono>

namespace interpreter::detail::uic::u_flex
{

  constexpr std::array<unsigned int, 12> nonLeapDaysOfMonth =
      {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  constexpr std::array<unsigned int, 12> leapDaysOfMonth =
      {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

  std::optional<std::string> daysAndYearToIsoDate(long const inYear, long const inDayOfYear)
  {
    auto year = inYear;
    auto dayOfYear = inDayOfYear;
    auto leap = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    auto maxDays = leap ? 366l : 365l;

    if (dayOfYear > maxDays)
    {
      year += 1;
      dayOfYear -= maxDays;

      leap = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
      maxDays = leap ? 366l : 365l;

      if (dayOfYear > maxDays)
      {
        throw std::runtime_error("Overflow of multiple years unsupported: " + std::to_string(inDayOfYear));
      }
    }

    auto const &daysOfMonth = leap ? leapDaysOfMonth : nonLeapDaysOfMonth;

    auto const smaller = std::find_if(daysOfMonth.rbegin(), daysOfMonth.rend(), [day = dayOfYear](auto v)
                                      { return v < day; });
    if (smaller == daysOfMonth.rend())
    {
      return std::nullopt;
    }

    auto const month = std::distance(smaller, daysOfMonth.rend());
    if (month == 0)
    {
      return std::nullopt;
    }

    unsigned int dayOfMonth = dayOfYear - *smaller;

    std::stringstream os;
    os << year << "-"
       << std::setw(2) << std::setfill('0') << month << "-"
       << std::setw(2) << std::setfill('0') << dayOfMonth;
    return os.str();
  }

  std::optional<std::string> daysAndYearToIsoDate(long const *const year, long const *const dayOfYear)
  {
    if (year == nullptr || dayOfYear == nullptr)
    {
      return std::nullopt;
    }

    return daysAndYearToIsoDate(*year, *dayOfYear);
  }

  std::string minutesToIsoTime(long const noOfMinutes)
  {

    // #if _LIBCPP_STD_VER >= 20
    //     return std::format("{:%T}", std::chrono::minutes(noOfMinutes));
    // #endif

    if (noOfMinutes < 0l)
    {
      return "00:00:00";
    }

    auto hours = noOfMinutes / 60l;
    auto const minutes = noOfMinutes - hours * 60l;
    if (hours > 23l)
    {
      auto const days = hours / 24l;
      hours = hours - days * 24l;
    }

    std::stringstream os;
    os << std::setw(2) << std::setfill('0') << hours << ":"
       << std::setw(2) << std::setfill('0') << minutes << ":"
       << "00";
    return os.str();
  }

  std::optional<std::string> minutesToIsoTime(long const *const noOfMinutes)
  {
    if (noOfMinutes == nullptr)
    {
      return std::nullopt;
    }

    return minutesToIsoTime(*noOfMinutes);
  }

  std::optional<std::string> quaterHoursToIsoZone(long const *const noOfQuaterHours)
  {
    if (noOfQuaterHours == nullptr)
    {
      return std::nullopt;
    }

    auto const noOfMinutes = *noOfQuaterHours * 15l;
    auto const hours = noOfMinutes / 60l;
    auto const minutes = noOfMinutes - hours * 60l;

    std::stringstream os;
    os << std::setw(3) << std::setfill('0') << std::internal << std::showpos << hours << ":"
       << std::setw(2) << std::setfill('0') << std::internal << std::noshowpos << minutes;
    return os.str();
  }
}
