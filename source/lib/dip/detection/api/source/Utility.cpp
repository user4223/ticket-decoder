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

    auto part = dip::filtering::toColor(source.clone());
    part(cv::Rect(0, 0, box.width, box.height)).copyTo(destination(box));
  }

  void visualize(cv::Mat &image, std::vector<Descriptor> const &contours)
  {
    image = filtering::toColor(std::move(image));
    std::for_each(contours.begin(), contours.end(), [&](auto const &contour)
                  { visualize(image, contour.image, contour.square); });
  }

  void visualize(cv::Mat &destination, std::vector<Descriptor> const &contours, bool overlayOutputImage)
  {
    destination = filtering::toColor(std::move(destination));
    std::for_each(contours.begin(), contours.end(), [&](auto const &d)
                  {
                    if (d.contour.empty()) 
                    {
                      return;
                    }

                    cv::polylines(destination, d.contour, true, red, 1);
                    std::for_each(d.annotators.begin(), d.annotators.end(), [&](auto const annotator)
                                  {
                                    auto const [position, text] = annotator(d);
                                    utility::putBlueText(destination, position, text); 
                                  }); });
  }
}
