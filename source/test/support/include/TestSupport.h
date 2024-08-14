
#pragma once

#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/utility/include/Logging.h"

#include <opencv2/core.hpp>

#include <string>
#include <vector>
#include <filesystem>

namespace test::support
{
  void init(int argc, char **argv);

  std::filesystem::path getExecutableFolderPath();

  std::unique_ptr<uic918::api::SignatureChecker> getSignatureChecker();

  std::vector<std::uint8_t> getData(std::string fileName);

  cv::Mat getImage(std::string fileName);

  cv::Mat getDummyImage(std::size_t widthHeight = 1);
}
