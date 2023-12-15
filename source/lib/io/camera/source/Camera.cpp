
#include "../include/Camera.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdexcept>

namespace io::camera
{
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

  api::InputElement readCamera(unsigned int device)
  {
    if (!camera)
    {
      camera = create(device);
    }
    cv::Mat image;
    camera->read(image);
    if (image.channels() == 1)
    {
      return api::InputElement::fromCamera(std::move(image.clone()));
    }

    cv::Mat output;
    cv::cvtColor(image, output, cv::COLOR_RGB2GRAY);
    return api::InputElement::fromCamera(std::move(output));
  }

  void releaseCamera()
  {
    if (camera)
    {
      camera->release();
      camera.release();
    }
  }
}
