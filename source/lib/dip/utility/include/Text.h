#pragma once

#include <opencv2/core.hpp>

#include <string>
#include <vector>

namespace dip::utility
{

  void drawBlueText(cv::Mat &image, cv::Point const &position, std::string text);

  void drawBlueText(cv::Mat &image, std::vector<std::tuple<cv::Point, std::string>> const &pointTextTuples);

  void drawRedText(cv::Mat &image, cv::Point const &position, std::string text);

  void drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines);

  void drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::string lines);

  void drawBlueDimensions(cv::Mat &image);

} // namespace dip::utility
