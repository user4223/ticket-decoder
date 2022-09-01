#include "../include/Descriptor.h"

namespace dip::filtering::pipe
{
  Descriptor Descriptor::fromImage(cv::Mat &&image)
  {
    auto descriptor = Descriptor{};
    descriptor.image = std::move(image);
    return descriptor;
  }

  Descriptor &&Descriptor::swap(Descriptor &&descriptor)
  {
    std::swap(descriptor.image, descriptor.shaddow);
    return std::move(descriptor);
  }
}
