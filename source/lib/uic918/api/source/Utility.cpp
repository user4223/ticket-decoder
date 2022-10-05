#include "../include/Utility.h"

#include <fstream>

namespace uic918::api
{
  void dump(std::filesystem::path const &basePath, std::string const &result)
  {
    if (!std::filesystem::exists(basePath.parent_path()))
    {
      std::filesystem::create_directories(basePath.parent_path());
    }

    auto const outputPath = basePath.string();
    if (result.compare("{}") != 0)
    {
      std::ofstream{outputPath + ".json", std::ios::binary}.write(
          result.data(), result.size());
    }
  }
}
