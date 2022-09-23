#pragma once

#include <opencv2/core.hpp>

#include <string>

namespace dip::utility
{

  void putBlueText(cv::Mat &image, std::string text, cv::Point const &position);

  void putRedText(cv::Mat &image, std::string text, cv::Point const &position);

  void putBlueDimensions(cv::Mat &image);

} // namespace dip::utility
