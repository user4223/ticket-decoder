#pragma once

#include "Detector.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <memory>

namespace dip::detection::api
{
  class ClassifierDetector : public Detector
  {
    ::utility::Logger logger;
    struct Internal;
    std::shared_ptr<Internal> internal; // by intention a shared_ptr 2 allow forward declared type here

  public:
    ClassifierDetector(utility::LoggerFactory &loggerFactory);

    Result detect(cv::Mat const &image) override;

    std::string getName() override;
  };
}
