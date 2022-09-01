#pragma once

#include "BarcodeDecodingLevel.h"

#include <opencv2/core.hpp>

#include <vector>
#include <ostream>

namespace barcode
{
  struct BarcodeDecodingResult
  {
    unsigned int id;
    cv::Rect box;
    BarcodeDecodingLevel level = BarcodeDecodingLevel::Unknown;
    std::vector<std::uint8_t> payload;

    BarcodeDecodingResult(unsigned int id, cv::Rect const &box);

    cv::Mat visualize(cv::Mat &&image) const;

    static BarcodeDecodingResult visualize(BarcodeDecodingResult &&result, std::ostream &stream);
  };
}
