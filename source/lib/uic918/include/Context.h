#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>

struct Context
{
  using BytesType = std::vector<std::uint8_t>;

  virtual ~Context() = default;

  virtual BytesType::const_iterator &getPosition() = 0;

  virtual bool isEmpty() = 0;

  virtual std::size_t getRemainingSize() = 0;

  virtual Context &addField(std::string key, std::string value) = 0;

  virtual Context &addField(std::string key, std::string value, std::string description) = 0;

  virtual Context &addField(std::string key, std::string value, std::optional<std::string> description) = 0;
};
