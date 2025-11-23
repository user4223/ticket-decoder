// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/FilterPipeDescriptor.h"

namespace detector::detail
{
  FilterPipeDescriptor FilterPipeDescriptor::fromImage(cv::Mat &&image)
  {
    auto descriptor = FilterPipeDescriptor{};
    descriptor.image = std::move(image);
    return descriptor;
  }

  FilterPipeDescriptor &&FilterPipeDescriptor::swap(FilterPipeDescriptor &&descriptor)
  {
    std::swap(descriptor.image, descriptor.shaddow);
    return std::move(descriptor);
  }
}
