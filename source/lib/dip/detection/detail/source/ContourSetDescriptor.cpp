
#include "../include/ContourSetDescriptor.h"

ContourSetDescriptor ContourSetDescriptor::fromContours(std::vector<ContourDescriptor> &&contours)
{
  auto descriptor = ContourSetDescriptor{};
  descriptor.contours = std::move(contours);
  return descriptor;
}

ContourSetDescriptor ContourSetDescriptor::fromContours(std::vector<ContourDescriptor::ContourType> &&contours)
{
  auto descriptor = ContourSetDescriptor{};
  descriptor.contours = ContourDescriptor::fromContours(std::move(contours));
  return descriptor;
}
