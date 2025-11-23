// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ShapeProperties.h"

#include <opencv2/core.hpp>

namespace dip
{
  void drawShape(cv::Mat &image, cv::Rect const &rectangle, ShapeProperties const &properties);

  void drawShape(cv::Mat &image, std::vector<cv::Point> const &shape, ShapeProperties const &properties);

  void drawRedShape(cv::Mat &image, std::vector<cv::Point> const &shape);
}
