#include "../include/SignatureChecker.h"

#include "lib/uic918/detail/include/UicSignatureChecker.h"

#include "lib/utility/include/Logging.h"

namespace uic918::api
{

  std::unique_ptr<SignatureChecker> SignatureChecker::create(
      ::utility::LoggerFactory &loggerFactory,
      std::filesystem::path const &uicSignatureXml)
  {
    return std::make_unique<detail::UicSignatureChecker>(loggerFactory, uicSignatureXml);
  }

  class Dummy : public api::SignatureChecker
  {
    ::utility::Logger logger;

  public:
    Dummy(::utility::LoggerFactory &loggerFactory)
        : logger(CREATE_LOGGER(loggerFactory))
    {
      LOG_WARN(logger) << "Using dummy signature checker";
    }

    virtual Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const override
    {
      return Result::KeyNotFound;
    }
  };

  std::unique_ptr<SignatureChecker> SignatureChecker::createDummy(
      ::utility::LoggerFactory &loggerFactory)
  {
    return std::make_unique<Dummy>(loggerFactory);
  }
}
