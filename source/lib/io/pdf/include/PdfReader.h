#pragma once

#include "lib/io/api/include/Reader.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::pdf
{

  class PdfReader : public io::api::Reader
  {
    ::utility::Logger logger;
    struct Internal;

    std::shared_ptr<Internal> internal; // shared to make forward decl type possible

  public:
    PdfReader(::utility::LoggerFactory &loggerFactory, unsigned int const dpi);

    std::vector<std::string> supportedFileExtensions() const override;

    cv::Mat read(std::filesystem::path path) const override;

    cv::Mat read(std::filesystem::path const &path, unsigned int const page) const;

    std::vector<cv::Mat> read(std::filesystem::path const &path, std::vector<unsigned int> pages) const;
  };
}
