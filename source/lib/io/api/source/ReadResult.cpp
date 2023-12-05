#include "../include/ReadResult.h"

namespace io::api
{

  ReadResult::ReadResult(cv::Mat &&i)
      : images(std::vector<cv::Mat>{std::move(i)})
  {
  }

  ReadResult::ReadResult(std::vector<cv::Mat> &&i)
      : images(std::move(i))
  {
    if (images.empty())
    {
      throw std::runtime_error("Expecting at least one image to create result from, got empty image list");
    }
  }

  bool ReadResult::isMultiPart() const
  {
    return images.size() > 1;
  }

  cv::Mat ReadResult::getImage()
  {
    if (isMultiPart())
    {
      throw std::runtime_error("Read result used as single part but it has a multi-part source and contains multiple images");
    }
    return images[0];
  }

  std::vector<cv::Mat> ReadResult::getImages()
  {
    return images;
  }
}
