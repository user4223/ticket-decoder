#include "../include/Utility.h"

#include "lib/dip/filtering/include/Transform.h"
#include "lib/dip/utility/include/Text.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace dip::detection::api
{
  static auto const red = cv::Scalar(0, 0, 255);

  void visualize(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box)
  {
    if (source.empty() || box == cv::Rect{})
    {
      return;
    }

    auto const &part = source.channels() == 3 ? source : dip::filtering::toColor(source.clone());
    part(cv::Rect(0, 0, box.width, box.height)).copyTo(destination(box));
  }

  void visualize(cv::Mat &destination, Descriptor::ContourType const &contour)
  {
    if (contour.empty())
    {
      return;
    }

    cv::polylines(destination, contour, true, red, 1);
  }

  void visualize(cv::Mat &destination, Descriptor const &descriptor)
  {
    std::for_each(descriptor.annotators.cbegin(), descriptor.annotators.cend(), [&](auto const &annotator)
                  {
                    auto const [position, text] = annotator(descriptor);
                    utility::putBlueText(destination, position, text); });
  }

  void visualize(cv::Mat &image, std::vector<Descriptor> const &descriptors, bool overlayOutputImage)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto const &descriptor)
                  { 
                    if (overlayOutputImage) visualize(image, descriptor.image, descriptor.square);
                    visualize(image, descriptor.contour);
                    visualize(image, descriptor); });
  }
}
