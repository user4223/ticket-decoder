#pragma once

#include <opencv2/core.hpp>

namespace dip::filtering
{

  cv::Mat toGray(cv::Mat const &input);

  cv::Mat toColor(cv::Mat &&input);

  cv::Mat rotate(cv::Mat const &input, float angle);

  cv::Mat flipX(cv::Mat const &input);

  cv::Mat flipY(cv::Mat const &input);

  cv::Mat flipXY(cv::Mat const &input);

  cv::Mat scale(cv::Mat const &input, float scale);

  cv::Rect split(cv::Size const &size, unsigned int partCount, unsigned int part);

  cv::Mat split(cv::Mat const &image, unsigned int partCount, unsigned int part);

  cv::Mat crazySquareBinarizer(cv::Mat const &image);

}
