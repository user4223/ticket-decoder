#include "../include/ImageDescriptor.h"

ImageDescriptor ImageDescriptor::fromImage(cv::Mat &&image)
{
  auto descriptor = ImageDescriptor{};
  descriptor.image = std::move(image);
  return descriptor;
}

ImageDescriptor &&ImageDescriptor::swap(ImageDescriptor &&descriptor)
{
  std::swap(descriptor.image, descriptor.shaddow);
  return std::move(descriptor);
}