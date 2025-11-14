#pragma once

#include "lib/input/detail/api/include/Reader.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

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
    PdfReader(infrastructure::Context &context, api::LoadOptions options);

    std::vector<std::string> supportedExtensions() const override;

    api::ReadResult read(std::filesystem::path path) const override;

    static std::vector<unsigned int> selectedPages(std::vector<unsigned int> const &pageIndexes, unsigned int pageCount);
  };
}
