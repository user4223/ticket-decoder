#pragma once

#include "BarcodeDecodingLevel.h"

#include <opencv2/core.hpp>

#include <vector>

struct BarcodeDecodingResult
{
  BarcodeDecodingLevel level = BarcodeDecodingLevel::Unknown;

  std::vector<std::uint8_t> payload;

  BarcodeDecodingResult(BarcodeDecodingLevel level);

  BarcodeDecodingResult(BarcodeDecodingLevel level, std::vector<std::uint8_t> &&payload);

  cv::Mat visualize(cv::Mat const &image);
};