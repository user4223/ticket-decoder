#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

/* TODO Rename to converter
 */
namespace uic918::detail::Utility
{
  std::string getAlphanumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t size);

  std::uint32_t getNumeric32(std::vector<std::uint8_t>::const_iterator &position);

  std::uint32_t getNumeric24(std::vector<std::uint8_t>::const_iterator &position);

  std::uint16_t getNumeric16(std::vector<std::uint8_t>::const_iterator &position);

  std::uint8_t getNumeric8(std::vector<std::uint8_t>::const_iterator &position);

  std::string getDateTimeCompact(std::vector<std::uint8_t>::const_iterator &position);

  std::string getDateTime12(std::vector<std::uint8_t>::const_iterator &position);

  std::string getDate8(std::vector<std::uint8_t>::const_iterator &position);

  std::vector<std::uint8_t> getBytes(std::vector<std::uint8_t>::const_iterator &position, std::size_t size);

  std::optional<std::string> toIsoDate(long const *const year, long const *const dayOfYear);
}
