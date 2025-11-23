// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <opencv2/core.hpp>

#include <optional>

namespace dip
{
  static auto const white = cv::Scalar(255, 255, 255);
  static auto const blue = cv::Scalar(255, 0, 0);
  static auto const green = cv::Scalar(0, 255, 0);
  static auto const red = cv::Scalar(0, 0, 255);
  static auto const yellow = cv::Scalar(0, 255, 255);
  static auto const cyan = cv::Scalar(255, 255, 0);

  static cv::Scalar colorOf(std::optional<bool> input)
  {
    if (!input)
    {
      return yellow;
    }
    return *input ? green : red;
  }
}
