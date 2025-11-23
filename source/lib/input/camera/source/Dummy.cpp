// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Camera.h"

#include <stdexcept>

namespace input::camera
{

  cv::Mat readCamera(unsigned int device)
  {
    throw std::runtime_error("Camera support disabled at compile-time");
  }

  void releaseCamera()
  {
  }
}
