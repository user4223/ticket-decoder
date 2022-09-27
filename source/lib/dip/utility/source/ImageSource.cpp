
#include "../include/ImageSource.h"

#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"

namespace dip::utility
{

  ImageSource::ImageSource(std::filesystem::path directory, unsigned int defaultSource)
      : imagePaths(::utility::scanForImages(directory)),
        inputSourceIndex(defaultSource > imagePaths.size() // 0 is camera
                             ? imagePaths.size()
                             : defaultSource)
  {
  }

  std::string ImageSource::nextSource()
  {
    return std::to_string(::utility::safeIncrement(inputSourceIndex, imagePaths.size()));
  }

  std::string ImageSource::previousSource()
  {
    return std::to_string(::utility::safeDecrement(inputSourceIndex));
  }

  std::optional<std::filesystem::path> ImageSource::getSource() const
  {
    return inputSourceIndex == 0 || imagePaths.empty()
               ? std::nullopt
               : std::make_optional(imagePaths[std::min((unsigned int)(imagePaths.size()), inputSourceIndex) - 1]);
  }

  ImageSource ImageSource::create(std::filesystem::path directory, unsigned int defaultSource)
  {
    return ImageSource(directory, defaultSource);
  }

}
