
#include "../include/ContourDescriptor.h"

#include <algorithm>
#include <sstream>

std::string ContourDescriptor::toString() const
{
  std::ostringstream os;
  os << "#" << std::to_string(id) << " [";
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
  int counter = 0;
  std::transform(contours.begin(), contours.end(), descriptors.begin(), [&counter](auto &&c)
                 { auto descriptor = ContourDescriptor{};
                   descriptor.contour = std::move(c);
                   descriptor.id =  counter++;
                   return descriptor; });
  return descriptors;
}

std::vector<ContourDescriptor> ContourDescriptor::fromRects(std::vector<cv::Rect> &&objects)
{
  auto descriptors = std::vector<ContourDescriptor>{objects.size()};
  int counter = 0;
  std::transform(objects.begin(), objects.end(), descriptors.begin(), [&counter](auto &&o)
                 { auto descriptor = ContourDescriptor{};
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

std::vector<ContourDescriptor::ContourType> ContourDescriptor::toContours(std::vector<ContourDescriptor> &&descriptors)
{
  auto contours = std::vector<ContourDescriptor::ContourType>{descriptors.size()};
  std::transform(descriptors.begin(), descriptors.end(), contours.begin(), [](auto &&d)
                 { return std::move(d.contour); });
  return contours;
}