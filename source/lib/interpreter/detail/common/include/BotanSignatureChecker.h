#pragma once

#include "Certificate.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include "lib/interpreter/api/include/SignatureChecker.h"

#include <map>
#include <string>
#include <filesystem>

namespace uic918::detail
{
  class BotanSignatureChecker : public api::SignatureChecker
  {
    ::utility::Logger logger;
    std::map<std::string, Certificate const> keys;

  public:
    BotanSignatureChecker(infrastructure::Context &context, std::filesystem::path const &uicSignatureXml);

    virtual api::SignatureChecker::Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const override;
  };
}
