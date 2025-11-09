#pragma once

#include <opencv2/core.hpp>

#include <optional>

namespace dip::filtering::pipe
{
  struct FilterPipeDescriptor
  {
    unsigned int stepCount = 0;
    cv::Mat image;
    cv::Mat shaddow;
    std::optional<cv::Mat> debugImage;

    static FilterPipeDescriptor fromImage(cv::Mat &&image);

    static FilterPipeDescriptor &&swap(FilterPipeDescriptor &&descriptor);
  };
}
