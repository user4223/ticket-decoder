
#include "../include/ImageSource.h"

#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"
#include "lib/utility/include/Logging.h"

#include "lib/dip/utility/include/Camera.h"
#include "lib/dip/utility/include/ImageCache.h"
#include "lib/dip/filtering/include/Transform.h"

namespace dip::utility
{
  static std::map<unsigned int, unsigned int> const partMapDefault = {{2u, 0u}, {4u, 0u}};

  ImageSource::ImageSource(::utility::LoggerFactory &loggerFactory, std::filesystem::path directory, unsigned int defaultSource, int defaultRotation)
      : logger(CREATE_LOGGER(loggerFactory)),
        imagePaths(::utility::scanForImages(directory)),
        inputSourceIndex(defaultSource > imagePaths.size() // 0 is camera
                             ? imagePaths.size()
                             : defaultSource),
        path(std::nullopt),
        annotation(),
        partMap({{2u, 0u}, {4u, 2u}}),
        parts(),
        rotationDegree(defaultRotation),
        scaleFactor(100u)
  {
    update();
  }

  void ImageSource::update()
  {
    path = inputSourceIndex == 0 || imagePaths.empty()
               ? std::nullopt
               : std::make_optional(imagePaths[std::min((unsigned int)(imagePaths.size()), inputSourceIndex) - 1]);
    annotation = path ? path->stem().string() : std::string("camera");
    parts = *std::max_element(partMap.begin(), partMap.end(),
                              [](auto const &a, auto const &b)
                              { return (std::min(1u, a.second) * a.first) < (std::min(1u, b.second) * b.first); });
  }

  std::string ImageSource::nextSource()
  {
    ::utility::safeIncrement(inputSourceIndex, imagePaths.size());
    update();
    return path ? path->filename() : "camera";
  }

  std::string ImageSource::previousSource()
  {
    ::utility::safeDecrement(inputSourceIndex, 0);
    update();
    return path ? path->filename() : "camera";
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
    ::utility::rotate(partMap.at(2), 2);
    update();
    return std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts));
  }

  std::string ImageSource::togglePart4()
  {
    ::utility::rotate(partMap.at(4), 4);
    update();
    return std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts));
  }

  std::string ImageSource::upScale()
  {
    return std::to_string(::utility::safeIncrement(scaleFactor, 200));
  }

  std::string ImageSource::downScale()
  {
    return std::to_string(::utility::safeDecrement(scaleFactor, 50));
  }

  std::string ImageSource::reset()
  {
    partMap = partMapDefault;
    rotationDegree = 0;
    scaleFactor = 100u;
    update();
    return "";
  }

  Source ImageSource::getSource() const
  {
    auto image = path ? dip::utility::getImage(*path) : dip::utility::readCamera();
    if (path)
    {
      dip::utility::releaseCamera();
      if (std::get<1>(parts) != 0)
      {
        image = dip::filtering::split(image, std::get<0>(parts), std::get<1>(parts));
      }
      if (path && rotationDegree != 0) // do not rotate when camera is used
      {
        image = dip::filtering::rotate(image, (float)rotationDegree);
      }
      if (path && scaleFactor != 100) // do not scale when camera is used
      {
        image = dip::filtering::scale(image, scaleFactor * 0.01f);
      }
    }
    return Source{path, annotation, std::move(image)};
  }

  ImageSource ImageSource::create(::utility::LoggerFactory &loggerFactory, std::filesystem::path directory, unsigned int defaultSource, int defaultRotation)
  {
    return ImageSource(loggerFactory, directory, defaultSource, defaultRotation);
  }
}
