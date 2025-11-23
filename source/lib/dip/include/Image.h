// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <opencv2/core.hpp>

namespace dip
{
  void copyTo(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box);

  std::vector<std::tuple<cv::Point, std::string>> getDimensionAnnotations(cv::Mat &image);
}
