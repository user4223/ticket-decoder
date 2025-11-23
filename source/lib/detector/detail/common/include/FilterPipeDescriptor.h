// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <opencv2/core.hpp>

#include <optional>

namespace detector::detail
{
  struct FilterPipeDescriptor
  {
    unsigned int stepCount = 0;
    cv::Mat image;
    cv::Mat shaddow;
    std::optional<cv::Mat> debugImage;

    static FilterPipeDescriptor fromImage(cv::Mat &&image);

    static FilterPipeDescriptor &&swap(FilterPipeDescriptor &&descriptor);
  };
}
