
#include "../include/PreProcessor.h"

#include "lib/utility/include/Utility.h"
#include "lib/utility/include/Logging.h"

#include "lib/dip/filtering/include/Transform.h"

namespace dip::utility
{
  std::pair<unsigned int, unsigned int> splitStringToPair(std::string input)
  {
    if (input.size() != 2)
    {
      return {1, 1};
    }
    auto const parts = input.at(0);
    auto const use = input.at(1);
    if (parts == '2')
    {
      if (use == '0' || use > '2')
      {
        return {1, 1};
      }
      return {2, use - '0'};
    }
    else if (parts == '4')
    {
      if (use == '0' || use > '4')
      {
        return {1, 1};
      }
      return {4, use - '0'};
    }
    return {1, 1};
  }

  static std::map<unsigned int, unsigned int> const partMapDefault = {{2u, 0u}, {4u, 0u}};

  std::map<unsigned int, unsigned int> splitPairToMap(std::pair<unsigned int, unsigned int> input)
  {
    auto result = partMapDefault;
    if (result.end() == result.find(input.first))
    {
      return result;
    }

    if (input.second == 0)
    {
      result[input.first] = 0;
    }
    else if (input.second > input.first)
    {
      result[input.first] = input.first;
    }
    else
    {
      result[input.first] = input.second;
    }
    return result;
  }

  PreProcessor::PreProcessor(::utility::LoggerFactory &loggerFactory, int defaultRotation, std::string defaultSplit)
      : logger(CREATE_LOGGER(loggerFactory)),
        partMap(splitPairToMap(splitStringToPair(defaultSplit))),
        parts(),
        rotationDegree(defaultRotation),
        scaleFactor(100u)
  {
    updatePartMap();
  }

  void PreProcessor::updatePartMap()
  {
    parts = *std::max_element(partMap.begin(), partMap.end(),
                              [](auto const &a, auto const &b)
                              { return (std::min(1u, a.second) * a.first) < (std::min(1u, b.second) * b.first); });
  }

  std::string PreProcessor::rotateClockwise()
  {
    return std::to_string(::utility::rotate(rotationDegree, -1, 360));
  }

  std::string PreProcessor::rotateCounterClockwise()
  {
    return std::to_string(::utility::rotate(rotationDegree, 1, 360));
  }

  std::string PreProcessor::togglePart2()
  {
    ::utility::rotate(partMap.at(2), 2);
    updatePartMap();
    return std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts));
  }

  std::string PreProcessor::togglePart4()
  {
    ::utility::rotate(partMap.at(4), 4);
    updatePartMap();
    return std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts));
  }

  std::string PreProcessor::upScale()
  {
    return std::to_string(::utility::safeIncrement(scaleFactor, 200));
  }

  std::string PreProcessor::downScale()
  {
    return std::to_string(::utility::safeDecrement(scaleFactor, 50));
  }

  std::string PreProcessor::reset()
  {
    partMap = partMapDefault;
    rotationDegree = 0;
    scaleFactor = 100u;
    updatePartMap();
    return "";
  }

  ::io::api::InputElement PreProcessor::get(::io::api::InputElement &&element)
  {
    if (!element.isValid())
    {
      return std::move(element);
    }

    auto image = element.getImage();
    if (std::get<1>(parts) != 0)
    {
      image = dip::filtering::split(image, std::get<0>(parts), std::get<1>(parts));
    }
    if (rotationDegree != 0)
    {
      image = dip::filtering::rotate(image, (float)rotationDegree);
    }
    if (scaleFactor != 100)
    {
      image = dip::filtering::scale(image, scaleFactor * 0.01f);
    }
    return ::io::api::InputElement::fromFile(element.getAnnotation(), std::move(image));
  }

  PreProcessor PreProcessor::create(
      ::utility::LoggerFactory &loggerFactory,
      int defaultRotation,
      std::string defaultSplit)
  {
    return PreProcessor(loggerFactory, defaultRotation, defaultSplit);
  }
}
