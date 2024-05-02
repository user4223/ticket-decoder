
#pragma once

#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/utility/include/Logging.h"

#include <opencv2/core.hpp>

#include <string>
#include <vector>
#include <filesystem>

namespace support
{
  class Loader
  {
    std::filesystem::path executableFolderPath;

  public:
    Loader(std::filesystem::path executableFolderPath);

    static void init(int argc, char **argv);

    static Loader &get();

    static std::filesystem::path getExecutableFolderPath();

    static std::unique_ptr<uic918::api::SignatureChecker> getSignatureChecker();

    static std::vector<std::uint8_t> getData(std::string fileName);

    static cv::Mat getImage(std::string fileName);
  };
}
