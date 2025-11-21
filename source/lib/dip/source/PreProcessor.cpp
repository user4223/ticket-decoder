
#include "../include/PreProcessor.h"
#include "../include/Transform.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include "lib/utility/include/Utility.h"

namespace dip
{
  PreProcessorOptions const PreProcessorOptions::DEFAULT = PreProcessorOptions{};

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

  PreProcessor::PreProcessor(infrastructure::Context &context, PreProcessorOptions o)
      : logger(CREATE_LOGGER(context.getLoggerFactory())),
        options(std::move(o)),
        isEnabled(true),
        partMap(splitPairToMap(splitStringToPair(options.split))),
        parts()
  {
    updatePartMap();
  }

  void PreProcessor::updatePartMap()
  {
    parts = *std::max_element(partMap.begin(), partMap.end(),
                              [](auto const &a, auto const &b)
                              { return (std::min(1u, a.second) * a.first) < (std::min(1u, b.second) * b.first); });
  }

  void PreProcessor::enable(bool e)
  {
    isEnabled = e;
    LOG_INFO(logger) << "Enabled: " << isEnabled;
  }

  std::string PreProcessor::rotateCW()
  {
    return std::to_string(::utility::rotate(options.rotationDegree, -1, 360));
  }

  std::string PreProcessor::rotateCCW()
  {
    return std::to_string(::utility::rotate(options.rotationDegree, 1, 360));
  }

  std::string PreProcessor::toggleSplit2()
  {
    ::utility::rotate(partMap.at(2), 2);
    updatePartMap();
    return std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts));
  }

  std::string PreProcessor::toggleSplit4()
  {
    ::utility::rotate(partMap.at(4), 4);
    updatePartMap();
    return std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts));
  }

  std::string PreProcessor::scaleUp()
  {
    return std::to_string(::utility::safeIncrement(options.scalePercent, 200));
  }

  std::string PreProcessor::scaleDown()
  {
    return std::to_string(::utility::safeDecrement(options.scalePercent, 50));
  }

  std::string PreProcessor::toggleFlipping()
  {
    return std::to_string(::utility::rotate(options.flippingMode, 3));
  }

  std::string PreProcessor::reset()
  {
    auto const defaultOptions = PreProcessorOptions{};
    partMap = splitPairToMap(splitStringToPair(defaultOptions.split));
    options.rotationDegree = defaultOptions.rotationDegree;
    options.scalePercent = defaultOptions.scalePercent;
    options.flippingMode = defaultOptions.flippingMode;
    updatePartMap();
    return "";
  }

  input::api::InputElement PreProcessor::get(input::api::InputElement &&element) const
  {
    if (!isEnabled || !element.isValid())
    {
      return std::move(element);
    }

    auto image = element.getImage();
    if (std::get<1>(parts) != 0)
    {
      image = dip::split(image, std::get<0>(parts), std::get<1>(parts));
    }
    if (options.flippingMode != 0)
    {
      switch (options.flippingMode)
      {
      case 1:
        image = dip::flipX(image);
        break;
      case 2:
        image = dip::flipY(image);
        break;
      case 3:
        image = dip::flipXY(image);
        break;
      }
    }
    if (options.rotationDegree != 0)
    {
      image = dip::rotate(image, (float)options.rotationDegree);
    }
    if (options.scalePercent != 100)
    {
      image = dip::scale(image, options.scalePercent * 0.01f);
    }

    return std::move(element.replaceImage(std::move(image)));
  }

  PreProcessor::ParameterTypeList PreProcessor::supplyParameters() const
  {
    if (!isEnabled)
    {
      return {};
    }
    return {std::make_pair("split:", std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts))),
            std::make_pair("rotate:", std::to_string(options.rotationDegree)),
            std::make_pair("scale:", std::to_string(options.scalePercent)),
            std::make_pair("flip:", std::to_string(options.flippingMode))};
  }

  PreProcessor PreProcessor::create(infrastructure::Context &context, PreProcessorOptions options)
  {
    return PreProcessor(context, std::move(options));
  }
}
