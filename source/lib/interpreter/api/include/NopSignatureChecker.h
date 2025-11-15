#pragma once

#include "SignatureChecker.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

namespace uic918::api
{
  /* Dummy implementation returning always KeyNotFound
   */
  class NopSignatureChecker : public SignatureChecker
  {
    ::utility::Logger logger;

  public:
    NopSignatureChecker(infrastructure::Context &context);

    static std::unique_ptr<SignatureChecker> create(infrastructure::Context &context);

    virtual Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const override;
  };
}
