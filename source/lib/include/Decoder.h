#pragma once

#include <vector>
#include <tuple>

class Decoder
{
public:
  enum class Level
  {
    Unknown,
    Detected,
    Decoded
  };

  virtual ~Decoder() = default;

  virtual Level detect() = 0;

  virtual std::tuple<Level, std::vector<std::uint8_t>> decode() = 0;
};