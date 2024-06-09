#pragma once

#include "lib/utility/include/LoggingFwd.h"

#include "lib/io/api/include/InputElement.h"

#include <opencv2/core.hpp>

#include <map>

namespace dip::filtering
{
  struct PreProcessorOptions
  {
    int rotationDegree;
    unsigned int scalePercent;
    std::string split;
  };

  std::pair<unsigned int, unsigned int> splitStringToPair(std::string input);

  std::map<unsigned int, unsigned int> splitPairToMap(std::pair<unsigned int, unsigned int> input);

  class PreProcessor
  {
    utility::Logger logger;
    PreProcessorOptions options;
    bool isEnabled;
    std::map<unsigned int, unsigned int> partMap;
    std::tuple<unsigned int, unsigned int> parts;

    PreProcessor(utility::LoggerFactory &loggerFactory, PreProcessorOptions options);

    void updatePartMap();

  public:
    void enable(bool enabled);

    std::string rotateCW();

    std::string rotateCCW();

    std::string toggleSplit2();

    std::string toggleSplit4();

    std::string scaleUp();

    std::string scaleDown();

    std::string reset();

    io::api::InputElement get(io::api::InputElement &&element) const;

    template <typename IteratorT>
    void toString(IteratorT inserter)
    {
      if (!isEnabled)
      {
        return;
      }
      *(inserter++) = std::make_pair("split:", std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts)));
      *(inserter++) = std::make_pair("rotation:", std::to_string(options.rotationDegree));
      *(inserter++) = std::make_pair("scale:", std::to_string(options.scalePercent));
    }

    static PreProcessor create(utility::LoggerFactory &loggerFactory, PreProcessorOptions options);
  };
}
