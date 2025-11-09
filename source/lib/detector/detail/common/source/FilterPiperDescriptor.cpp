#include "../include/FilterPipeDescriptor.h"

namespace dip::filtering::pipe
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
