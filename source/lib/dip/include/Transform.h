// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <opencv2/core.hpp>

namespace dip
{
  cv::Mat toGray(cv::Mat input);

  cv::Mat toColor(cv::Mat input);

  cv::Mat rotate(cv::Mat const &input, float angle);

  cv::Mat flipX(cv::Mat const &input);

  cv::Mat flipY(cv::Mat const &input);

  cv::Mat flipXY(cv::Mat const &input);

  cv::Mat scale(cv::Mat const &input, float scale);

  cv::Rect split(cv::Size const &size, unsigned int partCount, unsigned int part);

  cv::Mat split(cv::Mat const &image, unsigned int partCount, unsigned int part);
}
