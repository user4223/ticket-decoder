
#include "../include/CvUtility.h"

#include <opencv2/highgui.hpp>

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
