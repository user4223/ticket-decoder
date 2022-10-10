
#include "../include/Utility.h"

#include <algorithm>
#include <bit>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <array>

namespace uic918::detail::utility
{

  std::string getAlphanumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t size)
  {
    return std::string{position, std::find(position, position += size, '\0')};
  }

  template <typename T>
  T getNumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t sourceLength = sizeof(T))
  {
    if (sizeof(T) < sourceLength)
    {
      throw std::runtime_error("Destination size must be equal or greater than source size");
    }

    auto source = getBytes(position, sourceLength);
    auto destination = std::vector<std::uint8_t>(sizeof(T), 0);

    // TODO This depends on endianess, call revers only when endianess change between source and destination
    // if constexpr (std::endian::native == std::endian::big)
    // {
    //   throw std::runtime_error("Big endian machines not supported right now");
    // }
    std::reverse(source.begin(), source.end()); // TODO Big endian do not has to revert byte order
    auto const offset = 0;                      // destination.size() - source.size(); // TODO Big endian has to offset here

    std::copy(source.begin(), source.end(), destination.begin() + offset);
    return *(reinterpret_cast<T const *>(destination.data()));
  }

  std::uint32_t getNumeric32(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint32_t>(position);
  }

  std::uint32_t getNumeric24(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint32_t>(position, 3);
  }

  std::uint16_t getNumeric16(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint16_t>(position);
  }

  std::uint8_t getNumeric8(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint8_t>(position);
  }

  std::string getDateTimeCompact(std::vector<std::uint8_t>::const_iterator &position)
  {
    auto const date = getNumeric16(position);
    auto const time = getNumeric16(position);
    // TODO Use chrono parse or apply validation for all values
    std::ostringstream os;
    os << std::setw(4) << std::setfill('0') << std::to_string(((date & 0xFE00) >> 9) + 1990) << "-"
       << std::setw(2) << std::setfill('0') << std::to_string(((date & 0x01E0) >> 5)) << "-"
       << std::setw(2) << std::setfill('0') << std::to_string(((date & 0x001F) >> 0)) << "T"
       << std::setw(2) << std::setfill('0') << std::to_string(((time & 0xF800) >> 11)) << ":"
       << std::setw(2) << std::setfill('0') << std::to_string(((time & 0x07E0) >> 5)) << ":"
       << std::setw(2) << std::setfill('0') << std::to_string(((time & 0x001F) >> 0));
    return os.str();
  }

  std::string getDateTime12(std::vector<std::uint8_t>::const_iterator &position)
  {
    auto const input = getAlphanumeric(position, 12);
    auto const p = input.begin();
    // TODO Use chrono parse or apply validation for all values
    std::ostringstream os; // DDMMYYYYHHMM
    os << std::string(p + 4, p + 8) << "-"
       << std::string(p + 2, p + 4) << "-"
       << std::string(p + 0, p + 2) << "T"
       << std::string(p + 8, p + 10) << ":"
       << std::string(p + 10, p + 12) << ":"
       << "00";
    return os.str();
  }

  std::string getDate8(std::vector<std::uint8_t>::const_iterator &position)
  {
    auto const input = getAlphanumeric(position, 8);
    auto const p = input.begin();
    // TODO Use chrono parse or apply validation for all values
    std::ostringstream os; // DDMMYYYY
    os << std::string(p + 4, p + 8) << "-"
       << std::string(p + 2, p + 4) << "-"
       << std::string(p + 0, p + 2);
    return os.str();
  }

  std::vector<std::uint8_t> getBytes(std::vector<std::uint8_t>::const_iterator &position, std::size_t size)
  {
    return std::vector<std::uint8_t>{position, position += size};
  }

  constexpr std::array<unsigned int, 12> nonLeapDaysOfMonth =
      {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  constexpr std::array<unsigned int, 12> leapDaysOfMonth =
      {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

  std::optional<std::string> toIsoDate(long const *const year, long const *const dayOfYear)
  {
    if (year == nullptr || dayOfYear == nullptr)
    {
      return std::nullopt;
    }

    auto const &daysOfMonth = std::chrono::year(*year).is_leap()
                                  ? leapDaysOfMonth
                                  : nonLeapDaysOfMonth;

    auto const smaller = std::find_if(daysOfMonth.rbegin(), daysOfMonth.rend(), [day = *dayOfYear](auto v)
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
    unsigned int dayOfMonth = *dayOfYear - *smaller;

    std::stringstream os;
    os << *year << "-"
       << std::setw(2) << std::setfill('0') << month << "-"
       << std::setw(2) << std::setfill('0') << dayOfMonth;
    return std::make_optional(os.str());
  }
}