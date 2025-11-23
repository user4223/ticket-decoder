// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/DetectorPipeDescriptor.h"

namespace detector::detail
{
  DetectorPipeDescriptor DetectorPipeDescriptor::fromContours(std::vector<api::Descriptor> &&contours)
  {
    auto descriptor = DetectorPipeDescriptor{};
    descriptor.contours = std::move(contours);
    return descriptor;
  }

  DetectorPipeDescriptor DetectorPipeDescriptor::fromContours(std::vector<api::Descriptor::ContourType> &&contours)
  {
    auto descriptor = DetectorPipeDescriptor{};
    descriptor.contours = api::Descriptor::fromContours(std::move(contours));
    return descriptor;
  }
}
