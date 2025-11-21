#include "../include/SignatureVerifier.h"
#include "../include/NopSignatureVerifier.h"

#ifdef WITH_SIGNATURE_VERIFIER
#include "lib/interpreter/detail/verifier/include/BotanSignatureVerifier.h"
#endif

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

namespace interpreter::api
{
  std::unique_ptr<SignatureVerifier> SignatureVerifier::create(
      infrastructure::Context &context,
      std::filesystem::path const &uicSignatureXml)
  {
#ifdef WITH_SIGNATURE_VERIFIER
    return std::make_unique<detail::BotanSignatureVerifier>(context, uicSignatureXml);
#else
    return createDummy(context);
#endif
  }

  std::unique_ptr<SignatureVerifier> SignatureVerifier::createDummy(infrastructure::Context &context)
  {
    return std::make_unique<NopSignatureVerifier>(context);
  }
}
