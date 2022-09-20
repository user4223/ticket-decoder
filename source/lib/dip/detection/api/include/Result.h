#pragma once

#include "Descriptor.h"

#include <opencv2/core.hpp>

#include <vector>
#include <optional>

namespace dip::detection::api
{
  struct Result
  {
    using ContourType = std::vector<cv::Point>;

    std::vector<Descriptor> contours;
    std::optional<cv::Mat> debugImage;
    std::optional<std::vector<Descriptor>> debugContours;

    Result(std::vector<Descriptor> &&descriptors);

    Result(std::vector<Descriptor> &&descriptors, std::optional<cv::Mat> &&debugImage, std::optional<std::vector<Descriptor>> debugContours);
  };
}
