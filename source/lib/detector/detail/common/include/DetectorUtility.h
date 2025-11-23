// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <opencv2/core.hpp>

#include <vector>

namespace detector::detail
{
  std::vector<cv::Point> normalizePointOrder(std::vector<cv::Point> &&contour);

  cv::Point2f centerOf(std::vector<cv::Point> const &contour);

  cv::Point round(cv::Point2d const &source);
}
