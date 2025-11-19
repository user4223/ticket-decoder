
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
