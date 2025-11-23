// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/FilterPipe.h"

#include "lib/dip/include/Transform.h"

#include <opencv2/imgproc.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>

namespace detector::detail
{
  FilterPipe::FilterType FilterPipe::rotate(float angle)
  {
    return [angle](auto &&descriptor)
    {
      descriptor.shaddow = dip::rotate(descriptor.image, angle);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::resize(float factor, cv::Scalar const &color)
  {
    return [factor, &color](auto &&descriptor)
    {
      auto const vertical = factor * descriptor.image.rows;
      auto const horizontal = factor * descriptor.image.cols;
      cv::copyMakeBorder(descriptor.image, descriptor.shaddow, vertical, vertical, horizontal, horizontal, cv::BORDER_CONSTANT, color);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::smooth(int const kernelSize)
  {
    auto const kernel = cv::Size(kernelSize, kernelSize);
    return [kernel](auto &&descriptor)
    {
      cv::GaussianBlur(descriptor.image, descriptor.shaddow, kernel, 0);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::binarize()
  {
    // Otsu is probably faster because simpler than adaptive threshold, but global theshold is
    // only useful when input image is in well defined range and does not have huge
    // light changes globally.
    // So we could make this a parameter to choose between camera image and properly
    // scanned image to make a compromise between speed and quality of input when reasonable.
    return [](auto &&descriptor)
    {
      cv::threshold(descriptor.image, descriptor.shaddow, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::binarize(int const blockSize, int const substractFromMean)
  {
    return [blockSize, substractFromMean](auto &&descriptor)
    {
      cv::adaptiveThreshold(descriptor.image, descriptor.shaddow, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, substractFromMean);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::equalize()
  {
    return [](auto &&descriptor)
    {
      cv::equalizeHist(descriptor.image, descriptor.shaddow);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::edges(double threshold1, double threshold2, int aperture)
  {
    return [=](auto &&descriptor)
    {
      cv::Canny(descriptor.image, descriptor.shaddow, threshold1, threshold2, aperture);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::equalize(cv::Ptr<cv::CLAHE> const &clahe)
  {
    return [clahe](auto &&descriptor)
    {
      clahe->apply(descriptor.image, descriptor.shaddow);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::erode(cv::Mat const &kernel, int count)
  {
    return [&kernel, count](auto &&descriptor)
    {
      cv::erode(descriptor.image, descriptor.shaddow, kernel, cv::Point(-1, -1), count, cv::BORDER_CONSTANT, cv::Scalar(255));
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::dilate(cv::Mat const &kernel, int count)
  {
    return [&kernel, count](auto &&descriptor)
    {
      cv::dilate(descriptor.image, descriptor.shaddow, kernel, cv::Point(-1, -1), count, cv::BORDER_CONSTANT, cv::Scalar(255));
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::open(cv::Mat const &kernel, int count)
  {
    auto const anchor = cv::Point(-1, -1);
    return [&kernel, anchor, count](auto &&descriptor)
    {
      cv::morphologyEx(descriptor.image, descriptor.shaddow, cv::MorphTypes::MORPH_OPEN, kernel, anchor, count);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::close(cv::Mat const &kernel, int count)
  {
    auto const anchor = cv::Point(-1, -1);
    return [&kernel, anchor, count](auto &&descriptor)
    {
      cv::morphologyEx(descriptor.image, descriptor.shaddow, cv::MorphTypes::MORPH_CLOSE, kernel, anchor, count);
      return FilterPipeDescriptor::swap(std::move(descriptor));
    };
  }

  FilterPipe::FilterType FilterPipe::cloneInto(cv::Mat &image)
  {
    return [&](auto &&descriptor)
    {
      image = descriptor.image.clone();
      return std::move(descriptor);
    };
  }

  FilterPipeDescriptor FilterPipe::filter(FilterPipeDescriptor &&descriptor, std::vector<FilterType> &&filters)
  {
    return filter(std::move(descriptor), 0, std::move(filters));
  }

  FilterPipeDescriptor handleDebug(FilterPipeDescriptor &&input, unsigned int const debugStep)
  {
    if (++input.stepCount == debugStep)
    {
      input.debugImage = std::make_optional(input.image.clone());
    }
    return std::move(input);
  }

  FilterPipeDescriptor FilterPipe::filter(FilterPipeDescriptor &&descriptor, unsigned int const debugStep, std::vector<FilterType> &&filters)
  {
    return handleDebug(std::accumulate(filters.begin(), filters.end(), std::move(descriptor),
                                       [debugStep](auto &&input, auto const &filter) mutable
                                       { return filter(handleDebug(std::move(input), debugStep)); }),
                       debugStep);
  }
}
