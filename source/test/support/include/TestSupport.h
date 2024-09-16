
#pragma once

#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <string>
#include <vector>
#include <filesystem>

namespace test::support
{
  void init(int argc, char **argv);

  void uninit(); // only useful for explict context tests, don't use it!

  infrastructure::Context &getContext();

  ::utility::LoggerFactory &getLoggerFactory();

  std::filesystem::path getExecutableFolderPath();

  std::unique_ptr<uic918::api::SignatureChecker> getSignatureChecker();

  std::vector<std::uint8_t> getData(std::string fileName);

  cv::Mat getImage(std::string fileName);

  cv::Mat getDummyImage(std::size_t widthHeight = 1);

  class TempPath
  {
    std::filesystem::path path;

  public:
    TempPath(bool changeCurrentDirectory = true);
    ~TempPath();

    std::filesystem::path get() const { return path; }
  };
}
