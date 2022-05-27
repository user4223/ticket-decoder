#pragma once

#include <vector>
#include <tuple>
#include <cstdint>

class BarcodeDecoder
{
public:
  enum class Level
  {
    Unknown,
    Detected,
    Decoded
  };

  virtual ~BarcodeDecoder() = default;

  virtual Level detect() = 0;

  virtual std::tuple<Level, std::vector<std::uint8_t>> decode() = 0;
};