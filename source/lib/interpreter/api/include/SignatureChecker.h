#pragma once

#include "lib/infrastructure/include/ContextFwd.h"

#include <filesystem>
#include <vector>
#include <string>
#include <cstdint>

namespace uic918::api
{
  class SignatureChecker
  {
  public:
    enum class Result
    {
      KeyNotFound,
      Failed,
      Successful
    };

    static std::unique_ptr<SignatureChecker> create(infrastructure::Context &context, std::filesystem::path const &uicSignatureXml);

    /* Creates a dummy implementation returning always KeyNotFound
     */
    static std::unique_ptr<SignatureChecker> createDummy(infrastructure::Context &context);

    virtual ~SignatureChecker() = default;

    virtual Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const = 0;
  };
}
