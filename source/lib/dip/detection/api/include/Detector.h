#pragma once

#include "Result.h"

#include <opencv2/core.hpp>

namespace dip::detection::api
{
  class Detector
  {
  public:
    virtual ~Detector() = default;

    virtual Result detect(cv::Mat const &image) = 0;

    virtual std::string getName() = 0;
  };
}
