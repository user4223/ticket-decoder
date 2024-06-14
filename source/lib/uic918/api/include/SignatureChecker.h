#pragma once

#include "lib/utility/include/LoggingFwd.h"

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

    static std::unique_ptr<SignatureChecker> create(::utility::LoggerFactory &loggerFactory, std::filesystem::path const &uicSignatureXml);

    /* Creates a dummy implementation returning always KeyNotFound
     */
    static std::unique_ptr<SignatureChecker> createDummy(::utility::LoggerFactory &loggerFactory);

    virtual ~SignatureChecker() = default;

    virtual Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const = 0;
  };

}
