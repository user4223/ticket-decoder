
#include "../include/Loader.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/FileSystem.h"

#include "lib/dip/filtering/include/Transform.h"

#include <opencv2/imgcodecs.hpp>

#include <filesystem>

namespace support
{
  static std::unique_ptr<Loader> loaderHighlander;
  static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

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

  std::filesystem::path Loader::getExecutableFolderPath()
  {
    return get().executableFolderPath;
  }

  std::unique_ptr<uic918::api::SignatureChecker> Loader::getSignatureChecker()
  {
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
    return utility::readBinary(path);
  }

  cv::Mat Loader::getImage(std::string fileName)
  {
    auto const path = get().executableFolderPath / "etc" / "barcode" / fileName;
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    return dip::filtering::toGray(cv::imread(path.string()));
  }
}
