#pragma once

#include <memory>

#include <opencv2/core.hpp>

class Camera
{
public:
  static cv::Mat getImage(unsigned int device = 0);

  static void release();
};

class Splitter
{
public:
  static cv::Rect getPart(cv::Size const &size, unsigned int partCount, unsigned int part);
};
