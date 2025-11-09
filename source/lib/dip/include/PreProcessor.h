#pragma once

#include "lib/infrastructure/include/ParameterSupplier.h"
#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include "lib/io/api/include/InputElement.h"

#include <opencv2/core.hpp>

#include <map>

namespace dip::filtering
{
  struct PreProcessorOptions
  {
    int rotationDegree = 0;
    unsigned int scalePercent = 100u;
    std::string split = "11";
    unsigned int flippingMode = 0; // 0 nothing, 1 flip around X, 2 flip around Y, 3 flip around X and Y

    static PreProcessorOptions const DEFAULT;
  };

  std::pair<unsigned int, unsigned int> splitStringToPair(std::string input);

  std::map<unsigned int, unsigned int> splitPairToMap(std::pair<unsigned int, unsigned int> input);

  class PreProcessor : public infrastructure::ParameterSupplier
  {
    ::utility::Logger logger;
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

    io::api::InputElement get(io::api::InputElement &&element) const;

    ParameterTypeList supplyParameters() const;

    static PreProcessor create(infrastructure::Context &context, PreProcessorOptions options = {});
  };
}
