
#include "../include/TestSupport.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include "lib/input/common/include/Utility.h"

#include "lib/dip/include/Transform.h"

#include <opencv2/imgcodecs.hpp>

#include <filesystem>

namespace test::support
{
  struct State
  {
    std::filesystem::path const executableFolderPath;
    infrastructure::Context context;

    State(char **argv)
        : executableFolderPath(std::filesystem::current_path().append(argv[0]).parent_path()),
          context(::utility::LoggerFactory::createLazy(false))
    {
    }
  };

  static std::unique_ptr<State> state;

  void init(int argc, char **argv)
  {
    if (state.get() != nullptr)
    {
      throw std::runtime_error("Test support is initialized already and cannot be initialized twice");
    }
    state = std::make_unique<State>(argv);
  }

  void uninit()
  {
    state.reset();
  }

  infrastructure::Context &getContext()
  {
    return state->context;
  }

  ::utility::LoggerFactory &getLoggerFactory()
  {
    return getContext().getLoggerFactory();
  }

  std::filesystem::path getExecutableFolderPath()
  {
    return state->executableFolderPath;
  }

  std::unique_ptr<uic918::api::SignatureChecker> getSignatureChecker()
  {
    return uic918::api::SignatureChecker::create(
        getContext(),
        std::filesystem::current_path() / "cert" / "UIC_PublicKeys.xml");
  }

  std::vector<std::uint8_t> getData(std::string fileName)
  {
    auto const path = state->executableFolderPath / "etc" / "uic918" / fileName;
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    return io::api::utility::readBinary(path);
  }

  cv::Mat getImage(std::string fileName)
  {
    auto const path = state->executableFolderPath / "etc" / "barcode" / fileName;
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    return dip::filtering::toGray(cv::imread(path.string()));
  }

  static std::map<std::size_t, std::vector<std::uint8_t>> dummyImages;

  cv::Mat getDummyImage(std::size_t widthHeight)
  {
    auto item = dummyImages.find(widthHeight);
    if (item == dummyImages.end())
    {
      item = dummyImages.emplace(std::make_pair(widthHeight, std::vector<std::uint8_t>(widthHeight, 0))).first;
    }
    return cv::Mat{(int)widthHeight, (int)widthHeight, CV_8UC1, item->second.data()}.clone();
  }

  static const char charset[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  std::string generateRandomString(size_t length)
  {
    auto randchar = []() -> char
    {
      const size_t max_index = (sizeof(charset) - 1);
      return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
  }

  TempPath::TempPath(bool changeCurrentDirectory)
      : path(std::filesystem::temp_directory_path() / generateRandomString(20))
  {
    std::filesystem::create_directories(path);
    if (changeCurrentDirectory)
    {
      std::filesystem::current_path(path);
    }
  }

  TempPath::~TempPath()
  {
    std::error_code ec;
    std::filesystem::remove_all(path, ec);
  }
}
