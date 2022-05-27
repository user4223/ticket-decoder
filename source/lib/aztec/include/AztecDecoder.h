#pragma once

#include "BarcodeDecoder.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>
#include <tuple>

class AztecDecoder : public BarcodeDecoder
{
  struct Internal;

  std::shared_ptr<Internal> internal; // shared to make forward decl type possible

  AztecDecoder(std::shared_ptr<Internal> internal);

public:
  static std::unique_ptr<BarcodeDecoder> create(cv::Mat const &image, bool const pure);

  virtual Level detect() override;

  virtual std::tuple<Level, std::vector<std::uint8_t>> decode() override;
};