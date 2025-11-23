// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/detector/api/include/Descriptor.h"

#include <vector>
#include <functional>
#include <optional>

namespace detector::detail
{
  struct DetectorPipeDescriptor
  {
    unsigned int stepCount = 0;
    std::vector<api::Descriptor> contours;
    std::optional<std::vector<api::Descriptor>> debugContours;

    static DetectorPipeDescriptor fromContours(std::vector<api::Descriptor> &&contours);

    static DetectorPipeDescriptor fromContours(std::vector<api::Descriptor::ContourType> &&contours);

    void forEachContour(std::function<void(api::Descriptor &)> &&consumer)
    {
      std::for_each(contours.begin(), contours.end(), consumer);
    }
  };
}
