#include "../include/ReadResult.h"

namespace input::detail
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

  void ReadResult::ensureNoMultipart() const
  {
    if (isMultiPart())
    {
      throw std::runtime_error("Read result used as single part but it has a multi-part source and contains multiple images");
    }
  }

  cv::Mat ReadResult::getImage()
  {
    ensureNoMultipart();
    return images[0];
  }

  cv::Mat ReadResult::consumeImage()
  {
    ensureNoMultipart();
    return std::move(images[0]);
  }

  std::vector<cv::Mat> ReadResult::getImages()
  {
    return images;
  }

  std::vector<cv::Mat> ReadResult::consumeImages()
  {
    return std::move(images);
  }
}
