#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <functional>

/* Minimal abstraction 4 raw opencv image proc make using code concentrating
   onto the most important key points. Boiler plate code and statically
   set parameters are hidden behind this minimal interface allowing
   one liner filter chaining as well.

   This is explicitly NOT hiding all details, it's just to make usage as
   simple as possible to keep the mind free 4 domain details.
*/
class ImageProcessor
{
public:
  using FilterType = std::function<cv::Mat(cv::Mat &&)>;

  static cv::Mat toGray(cv::Mat const &input);

  static cv::Mat rotate(cv::Mat const &input, float angle);

  static FilterType smooth(int const kernelSize);

  static FilterType binarize();

  static FilterType binarize(int const blockSize, int const substractFromMean);

  static FilterType equalize();

  static FilterType equalize(cv::Ptr<cv::CLAHE> const &clahe);

  static FilterType edges(double threshold1, double threshold2, int aperture);

  static FilterType erode(cv::Mat const &kernel, int count);

  static FilterType dilate(cv::Mat const &kernel, int count);

  /* erode, then dilate */
  static FilterType open(cv::Mat const &kernel, int count);

  /* dilate, then erode */
  static FilterType close(cv::Mat const &kernel, int count);

  static FilterType cloneInto(cv::Mat &image);

  static cv::Mat filter(cv::Mat &&input, std::vector<FilterType> &&filters);
};
