
#include "../include/Loader.h"

#include "lib/utility/include/Logging.h"

#include <filesystem>
#include <fstream>

namespace support
{
  std::unique_ptr<uic918::api::SignatureChecker> getSignatureChecker()
  {
    auto loggerFactory = utility::LoggerFactory::create();
    return uic918::api::SignatureChecker::create(
        loggerFactory,
        std::filesystem::current_path() / ".." / ".." / ".." / "cert" / "UIC_PublicKeys_20221107.xml");
  }

  std::vector<std::uint8_t> getData(std::string fileName)
  {
    auto const path = std::filesystem::path("..").append("uic918").append("etc").append(fileName);
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    auto ifs = std::ifstream(path, std::ios::binary | std::ios::ate);
    auto const size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    auto buffer = std::vector<std::uint8_t>(size);
    ifs.read(reinterpret_cast<char *>(buffer.data()), size);
    return buffer;
  }
}
