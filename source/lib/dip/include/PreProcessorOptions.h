// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>

namespace dip
{
  class OptionsBuilder;

  struct PreProcessorOptions
  {
    int rotationDegree = 0;
    unsigned int scalePercent = 100u;
    std::string splittingMode = "11"; // 11 first of one part, 2x x of two parts, 4x x of four parts
    unsigned int flippingMode = 0;    // 0 nothing, 1 flip around X, 2 flip around Y, 3 flip around X and Y

    static PreProcessorOptions const DEFAULT;

    OptionsBuilder create();
  };

  class OptionsBuilder
  {
    PreProcessorOptions options;

    OptionsBuilder(PreProcessorOptions o);

  public:
    friend PreProcessorOptions;

    PreProcessorOptions build();
  };
}
