#include "../include/SignatureChecker.h"
#include "../include/NopSignatureChecker.h"

#include "lib/interpreter/detail/common/include/BotanSignatureChecker.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

namespace uic918::api
{
  std::unique_ptr<SignatureChecker> SignatureChecker::create(
      infrastructure::Context &context,
      std::filesystem::path const &uicSignatureXml)
  {
    return std::make_unique<detail::BotanSignatureChecker>(context, uicSignatureXml);
  }

  std::unique_ptr<SignatureChecker> SignatureChecker::createDummy(infrastructure::Context &context)
  {
    return std::make_unique<NopSignatureChecker>(context);
  }
}
