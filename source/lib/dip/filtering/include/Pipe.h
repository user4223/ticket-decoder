#pragma once

#include "Descriptor.h"

#include <opencv2/core.hpp>

#include <functional>

namespace cv
{
  class CLAHE;
}

namespace dip::filtering::pipe
{
  /* Minimal abstraction 4 raw opencv image proc make using code concentrating
     onto the most important key points. Boiler plate code and statically
     set parameters are hidden behind this minimal interface allowing
     one liner filter chaining as well.

     This is explicitly NOT hiding all details, it's just to make usage as
     simple as possible to keep the mind free 4 domain details.
  */
  using FilterType = std::function<Descriptor(Descriptor &&)>;

  FilterType rotate(float angle);

  FilterType resize(float factor, cv::Scalar const &color);

  FilterType smooth(int const kernelSize);

  FilterType binarize();

  FilterType binarize(int const blockSize, int const substractFromMean);

  FilterType equalize();

  FilterType equalize(cv::Ptr<cv::CLAHE> const &clahe);

  FilterType edges(double threshold1, double threshold2, int aperture);

  FilterType erode(cv::Mat const &kernel, int count);

  FilterType dilate(cv::Mat const &kernel, int count);

  /* erode, then dilate */
  FilterType open(cv::Mat const &kernel, int count);

  /* dilate, then erode */
  FilterType close(cv::Mat const &kernel, int count);

  FilterType cloneInto(cv::Mat &image);

  Descriptor filter(Descriptor &&descriptor, std::vector<FilterType> &&filters);

  Descriptor filter(Descriptor &&descriptor, unsigned int const debugStep, std::vector<FilterType> &&filters);
}
