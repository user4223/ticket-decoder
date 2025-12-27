// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/detector/api/include/Detector.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/DebugController.h"
#include "lib/infrastructure/include/Logger.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

namespace detector::detail
{
  class SquareDetector : public api::Detector
  {
    infrastructure::Logger logger;
    infrastructure::DebugController &debugController;
    api::DetectorOptions options;

  public:
    SquareDetector(infrastructure::Context &context, api::DetectorOptions options);

    bool isOperational() const override { return true; }

    api::Result detect(cv::Mat const &image) override;

    std::string getName() const override;

    api::DetectorType getType() const override;
  };
}
