#include "../include/SignatureVerifier.h"
#include "../include/NopSignatureVerifier.h"

#include "lib/interpreter/detail/common/include/BotanSignatureVerifier.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

namespace uic918::api
{
  std::unique_ptr<SignatureVerifier> SignatureVerifier::create(
      infrastructure::Context &context,
      std::filesystem::path const &uicSignatureXml)
  {
    return std::make_unique<detail::BotanSignatureVerifier>(context, uicSignatureXml);
  }

  std::unique_ptr<SignatureVerifier> SignatureVerifier::createDummy(infrastructure::Context &context)
  {
    return std::make_unique<NopSignatureVerifier>(context);
  }
}
