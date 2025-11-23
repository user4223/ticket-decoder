// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <opencv2/core.hpp>

#include <vector>
#include <string>
#include <optional>

namespace detector::api
{
  struct Descriptor
  {
    using ContourType = std::vector<cv::Point>;
    using AnnotationType = std::tuple<cv::Point, std::string>;
    using AnnotatorType = std::function<AnnotationType(Descriptor const &)>;

    unsigned int id;

    ContourType contour;
    cv::Rect square;
    cv::Mat image;

    std::vector<AnnotatorType> annotators;

    std::string toString() const;

    std::vector<AnnotationType> evaluateAnnotations() const;

    Descriptor clone() const
    {
      auto c = *this;
      c.image = image.clone();
      return c;
    }

    static std::vector<Descriptor> fromContours(std::vector<ContourType> &&contours);

    static std::vector<Descriptor> fromRects(std::vector<cv::Rect> &&rects);

    static std::vector<ContourType> toContours(std::vector<Descriptor> &&descriptors);
  };
}
