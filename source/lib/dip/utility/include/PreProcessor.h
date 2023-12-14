#pragma once

#include "lib/utility/include/LoggingFwd.h"

#include "lib/io/api/include/InputElement.h"

#include <opencv2/core.hpp>

#include <map>

namespace dip::utility
{
  std::pair<unsigned int, unsigned int> splitStringToPair(std::string input);

  std::map<unsigned int, unsigned int> splitPairToMap(std::pair<unsigned int, unsigned int> input);

  class PreProcessor
  {
    ::utility::Logger logger;
    std::map<unsigned int, unsigned int> partMap;
    std::tuple<unsigned int, unsigned int> parts;
    int rotationDegree;
    unsigned int scaleFactor;

    PreProcessor(::utility::LoggerFactory &loggerFactory, int defaultRotation, std::string defaultSplit);

    void updatePartMap();

  public:
    std::string rotateClockwise();

    std::string rotateCounterClockwise();

    std::string togglePart2();

    std::string togglePart4();

    std::string upScale();

    std::string downScale();

    std::string reset();

    ::io::api::InputElement get(::io::api::InputElement &&element);

    template <typename IteratorT>
    void toString(IteratorT inserter)
    {
      *(inserter++) = std::make_pair("split:", std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts)));
      *(inserter++) = std::make_pair("rotation:", std::to_string(rotationDegree));
      *(inserter++) = std::make_pair("scale:", std::to_string(scaleFactor));
    }

    static PreProcessor create(::utility::LoggerFactory &loggerFactory, int defaultRotation, std::string defaultSplit);
  };
}
