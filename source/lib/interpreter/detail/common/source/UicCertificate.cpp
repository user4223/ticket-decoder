
#include "../include/UicCertificate.h"

#include <botan/ber_dec.h>
#include <botan/base64.h>
#include <botan/x509_ca.h>
#include <botan/dsa.h>
#include <botan/pubkey.h>

#include <regex>
#include <map>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <memory>

namespace uic918::detail
{
  static auto const digitsOnly = std::regex("\\d+");
  static auto const alphanumericUpperOnly = std::regex("[\\dA-Z]+");
  static auto const sha__1Pattern = std::regex("sha[^\\d]?(1024|160)[^\\d]?", std::regex::icase);
  static auto const sha224Pattern = std::regex("sha[^\\d]?224[^\\d]?", std::regex::icase);
  static auto const sha256Pattern = std::regex("sha[^\\d]?(2048|256)[^\\d]?", std::regex::icase);

  static auto const sha1 = "EMSA1(SHA-160)";
  static auto const sha224 = "EMSA1(SHA-224)";
  static auto const sha256 = "EMSA1(SHA-256)";

  static auto const sha1Der46 = std::make_tuple(sha1, 46, Botan::Signature_Format::DER_SEQUENCE);
  static auto const sha224Der62 = std::make_tuple(sha224, 62, Botan::Signature_Format::DER_SEQUENCE);
  static auto const sha256Plain64 = std::make_tuple(sha256, 64, Botan::Signature_Format::IEEE_1363);

  using Config = std::tuple<std::string, std::uint8_t, Botan::Signature_Format>;

  static auto const signatureAlgorithmEmsaMap = std::map<std::string, Config>{
      {"SHA1withDSA(1024,160)", sha1Der46},
      {"SHA1withDSA", sha1Der46},
      {"SHA1-DSA (1024)", sha1Der46},
      {"SHA1-DSA (1024,160)", sha1Der46},
      {"DSA1024", sha1Der46},
      {"DSA_SHA1 (1024)", sha1Der46},
      {"(SHA-1, DSA 1024, Base16+11 Hexa Encoded (so stored as Ascii alphanumeric string), without ASN1)", sha1Der46}, // This is probably not correct
      //
      {"SHA224withDSA", sha224Der62},
      //
      {"SHA256withDSA(2048,256)", sha256Plain64},
      {"SHA256withECDSA-P256", sha256Plain64},
  };

  struct UicCertificate::Internal
  {
    std::string const id;
    std::string const issuer;
    std::string const algorithm;
    std::string const publicKey64;
    mutable std::unique_ptr<Botan::Public_Key> publicKey;
  };

  Botan::Public_Key const &getPublicKey(UicCertificate::Internal &internal)
  {
    if (internal.publicKey)
    {
      return *(internal.publicKey);
    }

    auto errors = std::vector<std::string>{};
    auto const publicKeyBytes = Botan::base64_decode(internal.publicKey64.data(), internal.publicKey64.size());
    try
    {
      auto dataSource = Botan::DataSource_Memory(publicKeyBytes);
      auto certificate = Botan::X509_Certificate(dataSource);
      internal.publicKey = certificate.load_subject_public_key();
      return *internal.publicKey;
    }
    catch (std::exception const &e)
    {
      errors.push_back(e.what());
    }

    try
    {
      auto dataSource = Botan::DataSource_Memory(publicKeyBytes);
      auto algorithmIdent = Botan::AlgorithmIdentifier();
      auto keyBits = std::vector<std::uint8_t>{};

      Botan::BER_Decoder(dataSource)
          .start_cons(Botan::ASN1_Tag::SEQUENCE)
          .decode(algorithmIdent)
          .decode(keyBits, Botan::ASN1_Tag::BIT_STRING)
          .end_cons();

      internal.publicKey = std::make_unique<Botan::DSA_PublicKey>(algorithmIdent, keyBits);
      return *internal.publicKey;
    }
    catch (std::exception const &e)
    {
      errors.push_back(e.what());
    }

    throw std::runtime_error(std::reduce(errors.begin(), errors.end(), std::string("Errors: "),
                                         [](auto &&result, auto const &error)
                                         {
                                           return result + ", " + error;
                                         }));
  }

  Config getConfig(std::string const &algorithm)
  {
    // Try direct map entry first
    auto const emsaEntry = signatureAlgorithmEmsaMap.find(algorithm);
    if (emsaEntry != signatureAlgorithmEmsaMap.end())
    {
      return emsaEntry->second;
    }

    // When first step fails, try to derive from detected strings
    if (std::regex_search(algorithm, sha__1Pattern))
    {
      return sha1Der46;
    }
    if (std::regex_search(algorithm, sha256Pattern))
    {
      return sha256Plain64;
    }
    if (std::regex_search(algorithm, sha224Pattern))
    {
      return sha224Der62;
    }

    throw std::runtime_error("No matching emsa value found for signatureAlgorithm: " + algorithm);
  }

  std::string UicCertificate::getNormalizedCode(std::string const &ricsCode)
  {
    if (ricsCode.empty())
    {
      throw std::runtime_error("Issuer code (RICS) empty");
    }
    if (ricsCode.size() > 4)
    {
      throw std::runtime_error("Issuer code (RICS) invalid, expecting 4 or less characters: " + ricsCode);
    }
    if (!std::regex_match(ricsCode, digitsOnly))
    {
      throw std::runtime_error("Issuer code (RICS) invalid, expecting digits only: " + ricsCode);
    }

    auto codeStream = std::stringstream{};
    codeStream << std::setw(4) << std::setfill('0') << ricsCode;
    return codeStream.str();
  }

  std::string UicCertificate::getNormalizedId(std::string const &keyId)
  {
    if (keyId.empty())
    {
      throw std::runtime_error("Key id empty");
    }
    if (keyId.size() > 5)
    {
      throw std::runtime_error("Key id invalid, expecting 5 or less characters: " + keyId);
    }
    if (!std::regex_match(keyId, alphanumericUpperOnly))
    {
      throw std::runtime_error("Key id invalid, expecting alphanumeric upper case only: " + keyId);
    }

    auto keyIdStream = std::stringstream{};
    keyIdStream << std::setw(5) << std::setfill('0') << keyId;
    return keyIdStream.str();
  }

  std::string UicCertificate::createMapKey(std::string const &ricsCode, std::string const &keyId)
  {
    auto const normalizedRicsCode = getNormalizedCode(ricsCode);
    auto const normalizedKeyId = keyId.size() == 9 && keyId.substr(0, 4).compare(normalizedRicsCode) == 0
                                     ? getNormalizedId(keyId.substr(4, 5))
                                     : getNormalizedId(keyId);

    auto stream = std::ostringstream{};
    stream << normalizedRicsCode << normalizedKeyId;
    return stream.str();
  }

  std::vector<std::uint8_t> UicCertificate::trimTrailingNulls(std::vector<std::uint8_t> const &buffer)
  {
    auto counter = 0u;
    auto const end = buffer.rend();
    for (auto current = buffer.rbegin(); current != end && *current == 0; ++current)
    {
      ++counter;
    }
    return {buffer.begin(), buffer.end() - counter};
  }

  std::string UicCertificate::getMapKey() const
  {
    return internal->id;
  }

  std::string UicCertificate::toString() const
  {
    auto stream = std::ostringstream{};
    stream << "Id: " << internal->id << ", "
           << "Issuer: " << internal->issuer << ", "
           << "Algorithm: " << internal->algorithm;
    return stream.str();
  }

  UicCertificate::UicCertificate(std::string ident, std::string issuerName, std::string signatureAlgorithm, std::string publicKey)
      : internal(new Internal{ident, issuerName, signatureAlgorithm, publicKey, {}})
  {
  }

  bool UicCertificate::verify(std::vector<std::uint8_t> const &message, std::vector<std::uint8_t> const &signature) const
  {
    auto const config = getConfig(internal->algorithm);
    auto const signatureLength = std::get<1>(config);
    if (signatureLength > signature.size())
    {
      throw std::runtime_error("Signature with length " + std::to_string(signature.size()) + " is shorter than expected, minimal expected: " + std::to_string(signatureLength));
    }
    auto verifier = Botan::PK_Verifier(getPublicKey(*internal), std::get<0>(config), std::get<2>(config));
    return verifier.verify_message(message, UicCertificate::trimTrailingNulls(signature));
  }
}
