#pragma once

#include "DetectorType.h"
#include "Result.h"
#include "DetectorOptions.h"

#include "lib/infrastructure/include/ParameterSupplier.h"
#include "lib/infrastructure/include/ContextFwd.h"

#include <opencv2/core.hpp>

#include <string>
#include <memory>
#include <map>

namespace dip::detection::api
{
  class Detector : public infrastructure::ParameterSupplier
  {
  public:
    virtual ~Detector() = default;

    virtual Result detect(cv::Mat const &image) = 0;

    virtual std::string getName() const = 0;

    virtual DetectorType getType() const = 0;

    virtual bool isOperational() const = 0;

    static std::unique_ptr<Detector> create(infrastructure::Context &context, DetectorType type, DetectorOptions options = {});

    static std::map<DetectorType, std::shared_ptr<Detector>> createAll(infrastructure::Context &context, DetectorOptions options = {});

    ParameterTypeList supplyParameters() const;
  };
}
