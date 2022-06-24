
#include "../include/CvUtility.h"

#include <opencv2/highgui.hpp>

#include <stdexcept>

static auto camera = std::unique_ptr<cv::VideoCapture>();

std::unique_ptr<cv::VideoCapture> create(unsigned int device)
{
  auto capture = std::make_unique<cv::VideoCapture>(device);
  if (!capture->isOpened())
  {
    throw std::runtime_error(std::string("Cannot open camera device: ") + std::to_string(device));
  }
  return capture;
}

cv::Mat Camera::getImage(unsigned int device)
{
  if (!camera)
  {
    camera = create(device);
  }
  cv::Mat image;
  (*camera) >> image;
  return image;
}

void Camera::release()
{
  if (camera)
  {
    camera->release();
  }
  camera.release();
}

cv::Rect Splitter::getPart(cv::Size const &size, unsigned int partCount, unsigned int part)
{
  if (part < 1 || part > partCount)
  {
    throw std::runtime_error(std::string("Part out of bounds: ") + std::to_string(part));
  }

  part -= 1;
  switch (partCount)
  {
  case 2:
  {
    auto const widthOverHeight = size.width >= size.height;
    auto const width = widthOverHeight ? size.width / 2 : size.width;
    auto const height = !widthOverHeight ? size.height / 2 : size.height;
    return cv::Rect(part * (widthOverHeight ? width : 0), part * (widthOverHeight ? 0 : height), width, height);
  }
  break;
  case 4:
  {
    auto const width = size.width / 2;
    auto const height = size.height / 2;
    return cv::Rect(part == 0 || part == 2 ? 0 : width, part == 0 || part == 1 ? 0 : height, width, height);
  }
  break;
  default:
    break;
  }

  throw std::runtime_error(std::string("Number of parts unsupported: ") + std::to_string(partCount));
}
