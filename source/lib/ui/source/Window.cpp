// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Window.h"

#include <opencv2/highgui.hpp>

namespace ui
{
  static bool created = false;

  void showImage(cv::Mat const &image)
  {
    if (!created)
    {
      cv::namedWindow("Output");
    }
    cv::imshow("Output", image);
  }
}
