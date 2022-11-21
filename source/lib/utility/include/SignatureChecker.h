#pragma once

#include "lib/utility/include/LoggingFwd.h"

#include <filesystem>
#include <memory>

namespace utility
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

    static std::unique_ptr<SignatureChecker> create(
        LoggerFactory &loggerFactory,
        std::filesystem::path const &uicSignatureXml);

    virtual ~SignatureChecker() = default;

    virtual Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const = 0;
  };

}
