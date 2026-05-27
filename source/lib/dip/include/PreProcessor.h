// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreProcessorOptions.h"

#include "lib/input/api/include/InputElement.h"

#include "lib/infrastructure/include/ParameterSupplier.h"
#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/Logger.h"

#include <opencv2/core.hpp>

#include <map>

namespace dip
{
  std::pair<unsigned int, unsigned int> splitStringToPair(std::string input);

  std::map<unsigned int, unsigned int> splitPairToMap(std::pair<unsigned int, unsigned int> input);

  class PreProcessor : public infrastructure::ParameterSupplier
  {
    infrastructure::Logger logger;
    PreProcessorOptions options;
    bool isEnabled;
    std::map<unsigned int, unsigned int> partMap;
    std::tuple<unsigned int, unsigned int> parts;

    PreProcessor(infrastructure::Context &context, PreProcessorOptions options);

    void updatePartMap();

  public:
    void enable(bool enabled);

    std::string rotateCW();

    std::string rotateCCW();

    std::string toggleSplit2();

    std::string toggleSplit4();

    std::string scaleUp();

    std::string scaleDown();

    std::string toggleFlipping();

    std::string reset();

    input::api::InputElement get(input::api::InputElement &&element) const;

    ParameterTypeList supplyParameters() const;

    static PreProcessor create(infrastructure::Context &context, PreProcessorOptions options = {});
  };
}
