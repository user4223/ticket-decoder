#pragma once

#include "DetectorOptions.h"
#include "Detector.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

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
    ClassifierDetector(infrastructure::Context &context, DetectorOptions options);

    virtual bool isOperational() const override;

    Result detect(cv::Mat const &image) override;

    std::string getName() const override;

    DetectorType getType() const override;
  };
}
