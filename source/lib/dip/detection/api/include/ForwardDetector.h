#pragma once

#include "Parameters.h"
#include "Detector.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

namespace dip::detection::api
{
  class ForwardDetector : public Detector
  {
    Parameters &parameters;

    ForwardDetector(Parameters &parameters);

  public:
    static std::unique_ptr<Detector> create(Parameters &parameters);

    Result detect(cv::Mat const &image) override;

    std::string getName() override;
  };
}
