#pragma once

#include "lib/io/api/include/Reader.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::pdf
{

  class PdfReader : public api::Reader
  {
    ::utility::Logger logger;
    struct Internal;

    std::shared_ptr<Internal> internal; // shared to make forward decl type possible

  public:
    PdfReader(::utility::LoggerFactory &loggerFactory, api::ReaderOptions options);

    std::vector<std::string> supportedExtensions() const override;

    api::ReadResult read(std::filesystem::path path) const override;

    static std::vector<unsigned int> selectedPages(std::vector<unsigned int> const &pageIndexes, unsigned int pageCount);
  };
}
