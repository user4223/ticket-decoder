
#include "../include/ContourUtility.h"

#include <opencv2/imgproc.hpp>

#include <cmath>

std::vector<cv::Point> ContourUtility::normalizePointOrder(std::vector<cv::Point> &&contour)
{
  if (contour.size() != 4)
  {
    throw std::runtime_error("Expecting contour having exactly 4 corner points but got: " + std::to_string(contour.size()));
  }

  std::sort(contour.begin(), contour.end(), [](auto const &a, auto const &b)
            { return a.x < b.x; });
  auto const [tl, bl] = contour[0].y < contour[1].y
                            ? std::make_tuple(contour[0], contour[1])
                            : std::make_tuple(contour[1], contour[0]);
  auto const [tr, br] = contour[2].y < contour[3].y
                            ? std::make_tuple(contour[2], contour[3])
                            : std::make_tuple(contour[3], contour[2]);

  return {tl, tr, br, bl};
}

cv::Point2f ContourUtility::centerOf(std::vector<cv::Point> const &contour)
{
  auto const moments = cv::moments(contour);
  auto const cX = (float)(moments.m10 / moments.m00);
  auto const cY = (float)(moments.m01 / moments.m00);
  return cv::Point2f(cX, cY);
}

cv::Point ContourUtility::round(cv::Point2d const &source)
{
  return cv::Point(std::round(source.x), std::round(source.y));
}
