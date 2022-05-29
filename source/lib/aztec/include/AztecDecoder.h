#pragma once

#include "BarcodeDecoder.h"

#include <opencv2/core/mat.hpp>

#include <memory>
#include <vector>
#include <tuple>

class AztecDecoder : public BarcodeDecoder
{
  struct Internal;

  std::shared_ptr<Internal> internal; // shared to make forward decl type possible

  AztecDecoder(std::shared_ptr<Internal> internal);

public:
  virtual BarcodeDecodingLevel detect() override;

  virtual BarcodeDecodingResult decode() override;

  static std::unique_ptr<BarcodeDecoder> create(cv::Mat const &image, bool const pure);

  static BarcodeDecodingResult decode(cv::Mat const &image, bool const pure);
};