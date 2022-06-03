#pragma once

#include "BarcodeDecodingLevel.h"
#include "BarcodeDecodingResult.h"

#include <vector>
#include <tuple>
#include <cstdint>

class BarcodeDecoder
{
public:
  virtual ~BarcodeDecoder() = default;

  virtual BarcodeDecodingLevel detect() = 0;

  virtual BarcodeDecodingResult decode() = 0;
};