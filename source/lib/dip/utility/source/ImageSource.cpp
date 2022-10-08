
#include "../include/ImageSource.h"

#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"

#include "lib/dip/utility/include/Camera.h"
#include "lib/dip/utility/include/ImageCache.h"
#include "lib/dip/filtering/include/Transform.h"

namespace dip::utility
{

  ImageSource::ImageSource(std::filesystem::path directory, unsigned int defaultSource)
      : imagePaths(::utility::scanForImages(directory)),
        inputSourceIndex(defaultSource > imagePaths.size() // 0 is camera
                             ? imagePaths.size()
                             : defaultSource),
        path(std::nullopt),
        annotation(),
        rotationDegree(0),
        parts({{2u, 0u}, {4u, 2u}}),
        scaleFactor(100u)
  {
    updatePath();
  }

  std::string ImageSource::updatePath()
  {
    path = inputSourceIndex == 0 || imagePaths.empty()
               ? std::nullopt
               : std::make_optional(imagePaths[std::min((unsigned int)(imagePaths.size()), inputSourceIndex) - 1]);
    annotation = path ? path->stem().string() : std::string("camera");
    return path ? path->filename() : "camera";
  }

  std::string ImageSource::nextSource()
  {
    ::utility::safeIncrement(inputSourceIndex, imagePaths.size());
    return updatePath();
  }

  std::string ImageSource::previousSource()
  {
    ::utility::safeDecrement(inputSourceIndex, 0);
    return updatePath();
  }

  std::string ImageSource::rotateClockwise()
  {
    return std::to_string(::utility::rotate(rotationDegree, -1, 360));
  }

  std::string ImageSource::rotateCounterClockwise()
  {
    return std::to_string(::utility::rotate(rotationDegree, 1, 360));
  }

  std::string ImageSource::togglePart2()
  {
    return std::to_string(::utility::rotate(parts.at(2), 2));
  }

  std::string ImageSource::togglePart4()
  {
    return std::to_string(::utility::rotate(parts.at(4), 4));
  }

  std::string ImageSource::upScale()
  {
    return std::to_string(::utility::safeIncrement(scaleFactor, 200));
  }

  std::string ImageSource::downScale()
  {
    return std::to_string(::utility::safeDecrement(scaleFactor, 50));
  }

  Source ImageSource::getSource() const
  {
    auto image = path ? dip::utility::getImage(*path) : dip::utility::readCamera();
    if (path)
    {
      dip::utility::releaseCamera();
      auto const [partCount, part] = *std::max_element(
          parts.begin(), parts.end(),
          [](auto const &a, auto const &b)
          { return (std::min(1u, a.second) * a.first) < (std::min(1u, b.second) * b.first); });

      if (part > 0)
      {
        image = dip::filtering::split(image, partCount, part);
      }
      if (rotationDegree > 0)
      {
        image = dip::filtering::rotate(image, (float)rotationDegree);
      }
      if (scaleFactor != 100)
      {
        image = dip::filtering::scale(image, scaleFactor * 0.01f);
      }
    }
    return Source{path, annotation, std::move(image)};
  }

  ImageSource ImageSource::create(std::filesystem::path directory, unsigned int defaultSource)
  {
    return ImageSource(directory, defaultSource);
  }
}
