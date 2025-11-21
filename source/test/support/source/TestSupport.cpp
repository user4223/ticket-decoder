
#include "../include/TestSupport.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include "lib/input/common/include/Utility.h"

#include "lib/dip/include/Transform.h"

#include "lib/interpreter/api/include/SignatureVerifier.h"

#include <opencv2/imgcodecs.hpp>

#include <filesystem>

namespace test::support
{
  static TestSupport *instance = nullptr;

  TestSupport &get()
  {
    return *instance;
  }

  TestSupport::TestSupport(int argc, char **argv)
      : TestSupport(std::filesystem::current_path().append(argv[0]).parent_path())
  {
  }

  TestSupport::TestSupport(std::filesystem::path ep)
      : executableFolderPath(ep),
        context(std::make_unique<infrastructure::Context>(::utility::LoggerFactory::createLazy(false)))
  {
    instance = this;
  }

  TestSupport::~TestSupport()
  {
    instance = nullptr;
  }

  struct TestSupport::ContextDisabler
  {
    std::unique_ptr<infrastructure::Context> &ref;

    ContextDisabler(std::unique_ptr<infrastructure::Context> &r) : ref(r)
    {
      ref.reset();
    }

    ~ContextDisabler()
    {
      ref = std::make_unique<infrastructure::Context>(::utility::LoggerFactory::createLazy(false));
    }
  };

  std::shared_ptr<TestSupport::ContextDisabler> TestSupport::disableContextTemporarily()
  {
    return std::make_shared<ContextDisabler>(context);
  }

  infrastructure::Context &TestSupport::getContext()
  {
    return *context;
  }

  ::utility::LoggerFactory &TestSupport::getLoggerFactory()
  {
    return getContext().getLoggerFactory();
  }

  std::filesystem::path TestSupport::getExecutableFolderPath()
  {
    return executableFolderPath;
  }

  std::filesystem::path TestSupport::getIOPath()
  {
    return executableFolderPath / "etc" / "io";
  }

  uic918::api::SignatureVerifier &TestSupport::getSignatureChecker()
  {
    if (!signatureVerifier)
    {
      signatureVerifier = uic918::api::SignatureVerifier::create(
          getContext(),
          std::filesystem::current_path() / "cert" / "UIC_PublicKeys.xml");
    }
    return *signatureVerifier;
  }

  std::vector<std::uint8_t> TestSupport::getInterpreterData(std::string fileName)
  {
    auto const path = executableFolderPath / "etc" / "interpreter" / fileName;
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    return input::detail::readBinary(path);
  }

  cv::Mat TestSupport::getDecoderImage(std::string fileName)
  {
    auto const path = executableFolderPath / "etc" / "decoder" / fileName;
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    return dip::filtering::toGray(cv::imread(path.string()));
  }

  cv::Mat TestSupport::getDummyImage(std::size_t widthHeight)
  {
    auto item = dummyImages.find(widthHeight);
    if (item == dummyImages.end())
    {
      item = dummyImages.emplace(std::make_pair(widthHeight, std::vector<std::uint8_t>(widthHeight, 0))).first;
    }
    return cv::Mat{(int)widthHeight, (int)widthHeight, CV_8UC1, item->second.data()}.clone();
  }
}
