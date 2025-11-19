#pragma once

#include "lib/detector/api/include/Detector.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include <opencv2/core.hpp>

#include <memory>

namespace detector::detail
{
  class ClassifierDetector : public api::Detector
  {
    ::utility::Logger logger;
    struct Internal;
    std::shared_ptr<Internal> internal; // by intention a shared_ptr 2 allow forward declared type here

  public:
    ClassifierDetector(infrastructure::Context &context, api::DetectorOptions options);

    virtual bool isOperational() const override;

    api::Result detect(cv::Mat const &image) override;

    std::string getName() const override;

    api::DetectorType getType() const override;
  };
}
