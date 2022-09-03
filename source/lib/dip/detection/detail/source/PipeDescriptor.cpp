
#include "../include/PipeDescriptor.h"

namespace dip::detection::detail
{
  PipeDescriptor PipeDescriptor::fromContours(std::vector<api::Descriptor> &&contours)
  {
    auto descriptor = PipeDescriptor{};
    descriptor.contours = std::move(contours);
    return descriptor;
  }

  PipeDescriptor PipeDescriptor::fromContours(std::vector<api::Descriptor::ContourType> &&contours)
  {
    auto descriptor = PipeDescriptor{};
    descriptor.contours = api::Descriptor::fromContours(std::move(contours));
    return descriptor;
  }
}
