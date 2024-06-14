
#include "../include/Camera.h"

#include "lib/utility/include/Logging.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdexcept>

namespace io::camera
{
  static auto loggerFactory = utility::LoggerFactory::createLazy(false);
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

    // TODO Source color space does not have to be always RGB, this breaks depending on camera and driver probably from here to now
    cv::Mat output;
    cv::cvtColor(image, output, cv::COLOR_RGB2GRAY);
    return api::InputElement::fromCamera(std::move(output));
  }

  void releaseCamera()
  {
    if (!camera)
    {
      return;
    }

    try
    {
      camera->release();
    }
    catch (std::exception const &ignore)
    {
      LOG_WARN(CREATE_LOGGER(loggerFactory)) << "Error while camera release: " << ignore.what();
    }
    camera.release();
  }
}
