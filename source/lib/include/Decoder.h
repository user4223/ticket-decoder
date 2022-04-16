#pragma once

#include <vector>
#include <tuple>

class Decoder
{
public:
  virtual ~Decoder() = default;

  virtual bool detect() = 0;

  virtual std::tuple<bool, std::vector<std::uint8_t>> decode() = 0;
};