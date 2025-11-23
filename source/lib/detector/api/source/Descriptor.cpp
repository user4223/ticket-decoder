// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Descriptor.h"

#include <algorithm>
#include <sstream>

namespace detector::api
{
  std::string Descriptor::toString() const
  {
    std::ostringstream os;
    os << "#" << std::to_string(id) << " [";
    /*if (!annotations.empty())
    {
      std::for_each(annotations.begin(), annotations.end() - 1, [&](auto const &a)
                    { os << a << ", "; });
      os << *(annotations.end() - 1);
    }*/
    os << "]";
    return os.str();
  }

  std::vector<Descriptor::AnnotationType> Descriptor::evaluateAnnotations() const
  {
    auto annotations = std::vector<AnnotationType>{};
    std::transform(annotators.begin(), annotators.end(), std::back_inserter(annotations),
                   [this](auto const annotator)
                   { return annotator(*this); });
    return annotations;
  }

  std::vector<Descriptor> Descriptor::fromContours(std::vector<Descriptor::ContourType> &&contours)
  {
    auto descriptors = std::vector<Descriptor>{contours.size()};
    int counter = 0;
    std::transform(contours.begin(), contours.end(), descriptors.begin(),
                   [&counter](auto &&c)
                   { auto descriptor = Descriptor{};
                   descriptor.contour = std::move(c);
                   descriptor.id =  counter++;
                   return descriptor; });
    return descriptors;
  }

  std::vector<Descriptor> Descriptor::fromRects(std::vector<cv::Rect> &&objects)
  {
    auto descriptors = std::vector<Descriptor>{objects.size()};
    int counter = 0;
    std::transform(objects.begin(), objects.end(), descriptors.begin(),
                   [&counter](auto &&o)
                   { auto descriptor = Descriptor{};
                   descriptor.contour = ContourType{
                      {o.x, o.y},                      // tl
                      {o.x + o.width, o.y},            // tr
                      {o.x + o.width, o.y + o.height}, // br
                      {o.x, o.y + o.height}};          // bl
                   descriptor.square = std::move(o);
                   descriptor.id = counter++; 
                   return descriptor; });
    return descriptors;
  }

  std::vector<Descriptor::ContourType> Descriptor::toContours(std::vector<Descriptor> &&descriptors)
  {
    auto contours = std::vector<Descriptor::ContourType>{descriptors.size()};
    std::transform(descriptors.begin(), descriptors.end(), contours.begin(),
                   [](auto &&d)
                   { return std::move(d.contour); });
    return contours;
  }
}
