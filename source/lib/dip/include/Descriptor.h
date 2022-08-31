#pragma once

#include <opencv2/core.hpp>

#include <optional>

namespace dip::pipe
{
  struct Descriptor
  {
    unsigned int stepCount = 0;
    cv::Mat image;
    cv::Mat shaddow;
    std::optional<cv::Mat> debugImage;

    static Descriptor fromImage(cv::Mat &&image);

    static Descriptor &&swap(Descriptor &&descriptor);
  };
}
