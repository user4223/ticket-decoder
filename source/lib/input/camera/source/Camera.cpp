// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Camera.h"

#include "lib/infrastructure/include/Logging.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdexcept>

namespace input::camera
{
  static auto loggerFactory = infrastructure::LoggerFactory::createLazy(false);
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

  cv::Mat readCamera(unsigned int device)
  {
    if (!camera)
    {
      camera = create(device);
    }

    cv::Mat image;
    camera->read(image);
    if (image.channels() == 1)
    {
      return image.clone();
    }

    // TODO Source color space does not have to be always RGB, this breaks depending on camera and driver probably from here to now
    cv::Mat output;
    cv::cvtColor(image, output, cv::COLOR_RGB2GRAY);
    return output;
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
