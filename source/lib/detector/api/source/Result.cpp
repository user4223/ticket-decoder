// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Result.h"

#include <functional>

namespace detector::api
{
  Result::Result(std::vector<Descriptor> &&d)
      : Result(std::move(d), std::optional<cv::Mat>{}, std::optional<std::vector<Descriptor>>{})
  {
  }

  Result::Result(std::vector<Descriptor> &&d, std::optional<cv::Mat> &&di, std::optional<std::vector<Descriptor>> dd)
      : contours(std::move(d)), debugImage(std::move(di)), debugContours(std::move(dd))
  {
  }

  size_t Result::for_each(std::function<void(Descriptor const &)> handler) const
  {
    std::for_each(contours.begin(), contours.end(), handler);
    return contours.size();
  }
}
