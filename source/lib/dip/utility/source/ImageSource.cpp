
#include "../include/ImageSource.h"

#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"

#include "lib/dip/utility/include/Camera.h"
#include "lib/dip/utility/include/ImageCache.h"

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

  Source ImageSource::getSource() const
  {
    auto path = inputSourceIndex == 0 || imagePaths.empty()
                    ? std::nullopt
                    : std::make_optional(imagePaths[std::min((unsigned int)(imagePaths.size()), inputSourceIndex) - 1]);
    if (path)
    {
      dip::utility::releaseCamera();
    }
    auto annotation = path ? std::make_optional(path->filename()) : std::nullopt;
    auto image = path ? dip::utility::getImage(*path) : dip::utility::readCamera();
    return Source{std::move(path), std::move(annotation), std::move(image)};
  }

  ImageSource ImageSource::create(std::filesystem::path directory, unsigned int defaultSource)
  {
    return ImageSource(directory, defaultSource);
  }
}
