
#include "../include/ContourDescriptor.h"

#include <algorithm>
#include <sstream>

std::string ContourDescriptor::toString() const
{
  std::ostringstream os;
  os << "[";
  if (!annotations.empty())
  {
    std::for_each(annotations.begin(), annotations.end() - 1, [&](auto const &a)
                  { os << a << ", "; });
    os << *(annotations.end() - 1);
  }
  os << "]";
  return os.str();
}

std::vector<ContourDescriptor> ContourDescriptor::fromContours(std::vector<ContourDescriptor::ContourType> &&contours)
{
  auto descriptors = std::vector<ContourDescriptor>{contours.size()};
  std::transform(contours.begin(), contours.end(), descriptors.begin(), [](auto &&c)
                 { 
                     auto descriptor = ContourDescriptor{};
                     descriptor.contour = std::move(c);
                     return descriptor; });
  return descriptors;
}

std::vector<ContourDescriptor::ContourType> ContourDescriptor::toContours(std::vector<ContourDescriptor> &&descriptors)
{
  auto contours = std::vector<ContourDescriptor::ContourType>{descriptors.size()};
  std::transform(descriptors.begin(), descriptors.end(), contours.begin(), [](auto &&d)
                 { return std::move(d.contour); });
  return contours;
}