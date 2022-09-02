
#include "../include/ContourSetDescriptor.h"

namespace dip::detection::detail
{
  ContourSetDescriptor ContourSetDescriptor::fromContours(std::vector<api::Descriptor> &&contours)
  {
    auto descriptor = ContourSetDescriptor{};
    descriptor.contours = std::move(contours);
    return descriptor;
  }

  ContourSetDescriptor ContourSetDescriptor::fromContours(std::vector<api::Descriptor::ContourType> &&contours)
  {
    auto descriptor = ContourSetDescriptor{};
    descriptor.contours = api::Descriptor::fromContours(std::move(contours));
    return descriptor;
  }
}
