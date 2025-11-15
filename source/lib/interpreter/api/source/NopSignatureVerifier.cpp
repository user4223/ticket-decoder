#include "../include/NopSignatureVerifier.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/utility/include/Logging.h"

namespace uic918::api
{
  NopSignatureVerifier::NopSignatureVerifier(infrastructure::Context &context)
      : logger(CREATE_LOGGER(context.getLoggerFactory()))
  {
    LOG_WARN(logger) << "Using dummy signature checker";
  }

  NopSignatureVerifier::Result NopSignatureVerifier::check(
      std::string const &ricsCode, std::string const &keyId,
      std::vector<std::uint8_t> const &message,
      std::vector<std::uint8_t> const &signature) const
  {
    return Result::KeyNotFound;
  }

  std::unique_ptr<SignatureVerifier> NopSignatureVerifier::create(infrastructure::Context &context)
  {
    return std::make_unique<NopSignatureVerifier>(context);
  }
}
