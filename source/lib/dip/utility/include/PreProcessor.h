#pragma once

#include <lib/utility/include/LoggingFwd.h>

#include <lib/io/api/include/Loader.h>

#include <opencv2/core.hpp>

#include <vector>
#include <optional>
#include <map>

namespace dip::utility
{
  std::pair<unsigned int, unsigned int> splitStringToPair(std::string input);

  std::map<unsigned int, unsigned int> splitPairToMap(std::pair<unsigned int, unsigned int> input);

  class PreProcessor
  {
    ::utility::Logger logger;
    ::io::api::LoadResult &loadResult;
    std::optional<::io::api::InputElement> currentElement = std::nullopt;
    unsigned int inputSourceIndex = 0u;
    std::map<unsigned int, unsigned int> partMap;
    std::tuple<unsigned int, unsigned int> parts;
    int rotationDegree;
    unsigned int scaleFactor;

    PreProcessor(
        ::utility::LoggerFactory &loggerFactory,
        ::io::api::LoadResult &loadResult,
        int defaultRotation,
        std::pair<unsigned int, unsigned int> defaultSplit);

    void updatePartMap();

  public:
    void refreshSources();

    std::string nextSource();

    std::string previousSource();

    std::string toggleCamera();

    std::string rotateClockwise();

    std::string rotateCounterClockwise();

    std::string togglePart2();

    std::string togglePart4();

    std::string upScale();

    std::string downScale();

    std::string reset();

    std::optional<::io::api::InputElement> get();

    template <typename IteratorT>
    void toString(IteratorT inserter)
    {
      *(inserter++) = std::make_pair("source:", currentElement.has_value() ? currentElement->getAnnotation() : "unknown");
      *(inserter++) = std::make_pair("split:", std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts)));
      *(inserter++) = std::make_pair("rotation:", std::to_string(rotationDegree));
      *(inserter++) = std::make_pair("scale:", std::to_string(scaleFactor));
    }

    static PreProcessor create(
        ::utility::LoggerFactory &loggerFactory,
        ::io::api::LoadResult &loadResult,
        int defaultRotation,
        std::pair<unsigned int, unsigned int> defaultSplit);
  };
}
