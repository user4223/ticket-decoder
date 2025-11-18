#pragma once

#include "FilterPipeDescriptor.h"

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
  class FilterPipe
  {
  public:
    using FilterType = std::function<FilterPipeDescriptor(FilterPipeDescriptor &&)>;

    static FilterType rotate(float angle);

    static FilterType resize(float factor, cv::Scalar const &color);

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

    static FilterPipeDescriptor filter(FilterPipeDescriptor &&descriptor, std::vector<FilterType> &&filters);

    static FilterPipeDescriptor filter(FilterPipeDescriptor &&descriptor, unsigned int const debugStep, std::vector<FilterType> &&filters);
  };
}
