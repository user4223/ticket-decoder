#pragma once

#include <opencv2/core.hpp>

#include <vector>

namespace io::api
{
  class ReadResult
  {
    std::vector<cv::Mat> images;

  public:
    ReadResult(cv::Mat &&image);
    ReadResult(std::vector<cv::Mat> &&images);

    ReadResult(ReadResult const &) = delete;
    ReadResult(ReadResult &&) = default;
    ReadResult &operator=(ReadResult const &) = delete;
    ReadResult &operator=(ReadResult &&) = default;

    /* Returns true when the read file contained more than one image and
       multiple images has been loaded, this could be the case e.g. for multi-page PDF files.
     */
    bool isMultiPart() const;

    /* Returns just the image in case of isMultiPart() == false
       and throws std::runtime_error in case of isMultiPart() == true
     */
    cv::Mat getImage();

    /* Returns a list of images loaded from file. In most cases a
       list of size 1, but e.g. in case of multi-page PDF files, each
       page results in one separated image.
     */
    std::vector<cv::Mat> getImages();
  };
}
