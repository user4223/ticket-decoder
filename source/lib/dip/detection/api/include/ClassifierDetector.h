#pragma once

#include "Detector.h"

#include <opencv2/core.hpp>

#include <memory>

namespace dip::detection::api
{
  class ClassifierDetector : public Detector
  {
    struct Internal;
    std::shared_ptr<Internal> internal; // by intention a shared_ptr 2 allow forward declared type here

    ClassifierDetector();

  public:
    static std::unique_ptr<Detector> create();

    Result detect(cv::Mat const &image) override;
  };
}
