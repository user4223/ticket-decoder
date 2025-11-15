#include "../include/NopSignatureChecker.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/utility/include/Logging.h"

namespace uic918::api
{
  NopSignatureChecker::NopSignatureChecker(infrastructure::Context &context)
      : logger(CREATE_LOGGER(context.getLoggerFactory()))
  {
    LOG_WARN(logger) << "Using dummy signature checker";
  }

  NopSignatureChecker::Result NopSignatureChecker::check(
      std::string const &ricsCode, std::string const &keyId,
      std::vector<std::uint8_t> const &message,
      std::vector<std::uint8_t> const &signature) const
  {
    return Result::KeyNotFound;
  }

  std::unique_ptr<SignatureChecker> NopSignatureChecker::create(infrastructure::Context &context)
  {
    return std::make_unique<NopSignatureChecker>(context);
  }
}
