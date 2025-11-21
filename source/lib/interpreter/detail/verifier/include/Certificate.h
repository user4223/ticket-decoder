#pragma once

#include <string>
#include <memory>
#include <vector>
#include <tuple>

namespace interpreter::detail
{
  struct Certificate
  {
    struct Internal;

    std::shared_ptr<Internal> internal;

    static std::string getNormalizedCode(std::string const &ricsCode);

    static std::string getNormalizedId(std::string const &keyId);

    static std::string createMapKey(std::string const &ricsCode, std::string const &keyId);

    static std::vector<std::uint8_t> trimTrailingNulls(std::vector<std::uint8_t> const &buffer);

    std::string getMapKey() const;

    std::string toString() const;

    Certificate(std::string ident, std::string issuerName, std::string signatureAlgorithm, std::string publicKey);

    bool verify(std::vector<std::uint8_t> const &message, std::vector<std::uint8_t> const &signature) const;
  };
}
