// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Descriptor.h"

#include <opencv2/core.hpp>

#include <vector>
#include <optional>
#include <functional>

namespace detector::api
{
  struct Result
  {
    using ContourType = std::vector<cv::Point>;

    // TODO Make this private
    std::vector<Descriptor> contours;
    std::optional<cv::Mat> debugImage;
    std::optional<std::vector<Descriptor>> debugContours;

    Result(std::vector<Descriptor> &&descriptors);

    Result(std::vector<Descriptor> &&descriptors, std::optional<cv::Mat> &&debugImage, std::optional<std::vector<Descriptor>> debugContours);

    size_t for_each(std::function<void(Descriptor const &)> handler) const;
  };
}
