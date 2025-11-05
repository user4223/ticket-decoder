#include "../include/SignatureChecker.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/interpreter/detail/common/include/UicSignatureChecker.h"

#include "lib/utility/include/Logging.h"

namespace uic918::api
{

  std::unique_ptr<SignatureChecker> SignatureChecker::create(
      infrastructure::Context &context,
      std::filesystem::path const &uicSignatureXml)
  {
    return std::make_unique<detail::UicSignatureChecker>(context, uicSignatureXml);
  }

  class Dummy : public api::SignatureChecker
  {
    ::utility::Logger logger;

  public:
    Dummy(infrastructure::Context &context)
        : logger(CREATE_LOGGER(context.getLoggerFactory()))
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

  std::unique_ptr<SignatureChecker> SignatureChecker::createDummy(infrastructure::Context &context)
  {
    return std::make_unique<Dummy>(context);
  }
}
