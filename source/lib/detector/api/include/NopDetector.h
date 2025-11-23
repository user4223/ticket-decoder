// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/detector/api/include/Detector.h"

#include "lib/infrastructure/include/ContextFwd.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

namespace detector::api
{
  /* Minimal NOP or pass through or forward detector doing nothing.
   */
  class NopDetector : public Detector
  {
    DetectorOptions options;

  public:
    NopDetector(infrastructure::Context &context, DetectorOptions options);

    bool isOperational() const override { return true; }

    Result detect(cv::Mat const &image) override;

    std::string getName() const override;

    DetectorType getType() const override;
  };
}
