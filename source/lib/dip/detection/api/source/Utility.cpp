#include "../include/Utility.h"

#include "lib/dip/filtering/include/Transform.h"
#include "lib/dip/utility/include/Text.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace dip::detection::api
{
  static auto const red = cv::Scalar(0, 0, 255);

  void visualize(cv::Mat &destination, std::vector<Descriptor> const &contours, bool overlayOutputImage)
  {
    destination = filtering::toColor(std::move(destination));
    std::for_each(contours.begin(), contours.end(), [&](auto const &d)
                  {
                    if (d.contour.empty()) 
                    {
                      return;
                    }

                    if (overlayOutputImage && !d.image.empty())
                    {
                      auto part = dip::filtering::toColor(d.image.clone());
                      part(cv::Rect(0, 0, d.square.width, d.square.height)).copyTo(destination(d.square));
                    }

                    cv::polylines(destination, d.contour, true, red, 1);
                    std::for_each(d.annotators.begin(), d.annotators.end(), [&](auto const annotator)
                                  {
                                    auto const [position, text] = annotator(d);
                                    utility::putBlueText(destination, text, position); 
                                  }); });
  }
}
