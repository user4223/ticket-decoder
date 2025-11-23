// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/DetectorUtility.h"

#include <opencv2/imgproc.hpp>

#include <cmath>

namespace detector::detail
{
  std::vector<cv::Point> normalizePointOrder(std::vector<cv::Point> &&contour)
  {
    if (contour.size() != 4)
    {
      throw std::runtime_error("Expecting contour having exactly 4 corner points but got: " + std::to_string(contour.size()));
    }

    std::sort(contour.begin(), contour.end(), [](auto const &a, auto const &b)
              { return a.x < b.x; });
    auto const [tl, bl] = contour[0].y < contour[1].y
                              ? std::make_tuple(contour[0], contour[1])
                              : std::make_tuple(contour[1], contour[0]);
    auto const [tr, br] = contour[2].y < contour[3].y
                              ? std::make_tuple(contour[2], contour[3])
                              : std::make_tuple(contour[3], contour[2]);

    return {tl, tr, br, bl};
  }

  cv::Point2f centerOf(std::vector<cv::Point> const &contour)
  {
    auto const moments = cv::moments(contour);
    auto const cX = (float)(moments.m10 / moments.m00);
    auto const cY = (float)(moments.m01 / moments.m00);
    return cv::Point2f(cX, cY);
  }

  cv::Point round(cv::Point2d const &source)
  {
    return cv::Point(std::round(source.x), std::round(source.y));
  }
}
