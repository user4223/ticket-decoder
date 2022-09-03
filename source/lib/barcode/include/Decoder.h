#pragma once

#include "BarcodeDecodingLevel.h"
#include "BarcodeDecodingResult.h"

#include <vector>
#include <tuple>
#include <cstdint>

namespace barcode::api
{
  class Decoder
  {
  public:
    virtual ~Decoder() = default;

    virtual BarcodeDecodingLevel detect() = 0;

    virtual BarcodeDecodingResult decode() = 0;

    static std::unique_ptr<Decoder> create(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure);

    static api::BarcodeDecodingResult decode(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure);
  };
}
