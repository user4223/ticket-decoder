#pragma once

#include <opencv2/core.hpp>

#include <string>
#include <vector>

namespace dip::utility
{

  void putBlueText(cv::Mat &image, cv::Point const &position, std::string text);

  void putRedText(cv::Mat &image, cv::Point const &position, std::string text);

  void putRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines);

  void putBlueDimensions(cv::Mat &image);

} // namespace dip::utility
