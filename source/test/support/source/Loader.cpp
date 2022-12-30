
#include "../include/Loader.h"

#include "lib/utility/include/Logging.h"

#include "lib/dip/filtering/include/Transform.h"
#include "lib/dip/utility/include/ImageCache.h"

#include <filesystem>
#include <fstream>

namespace support
{
  static std::unique_ptr<Loader> loaderHighlander;

  Loader::Loader(std::filesystem::path pfp) : executableFolderPath(std::move(pfp)) {}

  void Loader::init(int argc, char **argv)
  {
    loaderHighlander = std::make_unique<Loader>(std::filesystem::current_path().append(argv[0]).parent_path());
  }

  Loader &Loader::get()
  {
    if (!loaderHighlander)
    {
      throw std::runtime_error("Uninitialized loader");
    }
    return *loaderHighlander;
  }

  std::unique_ptr<uic918::api::SignatureChecker> Loader::getSignatureChecker()
  {
    auto loggerFactory = utility::LoggerFactory::create();
    return uic918::api::SignatureChecker::create(
        loggerFactory,
        std::filesystem::current_path() / "cert" / "UIC_PublicKeys.xml");
  }

  std::vector<std::uint8_t> Loader::getData(std::string fileName)
  {
    auto const path = get().executableFolderPath / "etc" / "uic918" / fileName;
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

  cv::Mat Loader::getImage(std::string fileName)
  {
    auto const path = get().executableFolderPath / "etc" / "barcode" / fileName;
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    return dip::filtering::toGray(dip::utility::getImage(path));
  }
}
