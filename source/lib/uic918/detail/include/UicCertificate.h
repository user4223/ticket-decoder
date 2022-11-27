#pragma once

#include <string>
#include <memory>
#include <vector>

#include <botan/pubkey.h>

namespace uic918::detail
{
  struct UicCertificate
  {
    std::string const ricsCode;
    std::string const keyId;
    std::string const issuer;
    std::string const algorithm;
    std::string const emsa;
    std::string const publicKey64;
    mutable std::unique_ptr<Botan::Public_Key> publicKey; // lazy load from base64 string

    static std::string getNormalizedCode(std::string const &ricsCode);

    static std::string getNormalizedId(std::string const &keyId);

    static std::string getEmsa(std::string const &algorithm);

    static std::string createMapKey(std::string const &ricsCode, std::string const &keyId);

    std::string getMapKey() const;

    std::string toString() const;

    Botan::Public_Key const &getPublicKey() const;

    bool verify(std::vector<std::uint8_t> const &message, std::vector<std::uint8_t> const &signature) const;
  };
}
