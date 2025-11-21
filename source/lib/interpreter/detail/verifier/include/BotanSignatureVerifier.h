#pragma once

#include "Certificate.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include "lib/interpreter/api/include/SignatureVerifier.h"

#include <map>
#include <string>
#include <filesystem>

namespace interpreter::detail
{
  class BotanSignatureVerifier : public api::SignatureVerifier
  {
    ::utility::Logger logger;
    std::map<std::string, Certificate const> keys;

  public:
    BotanSignatureVerifier(infrastructure::Context &context, std::filesystem::path const &uicSignatureXml);

    virtual api::SignatureVerifier::Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const override;
  };
}
