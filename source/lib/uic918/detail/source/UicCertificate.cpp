
#include "../include/UicCertificate.h"

#include <botan/ber_dec.h>
#include <botan/base64.h>
#include <botan/x509_ca.h>
#include <botan/dsa.h>

#include <regex>
#include <map>
#include <sstream>
#include <iomanip>
#include <numeric>

namespace uic918::detail
{
  static auto const digitsOnly = std::regex("\\d+");
  static auto const alphanumericUpperOnly = std::regex("[\\dA-Z]+");
  static auto const sha__1Pattern = std::regex("sha[^\\d]?(1024|160)[^\\d]?", std::regex::icase);
  static auto const sha224Pattern = std::regex("sha[^\\d]?224[^\\d]?", std::regex::icase);
  static auto const sha256Pattern = std::regex("sha[^\\d]?(2048|256)[^\\d]?", std::regex::icase);

  static auto const sha__1 = "EMSA1(SHA-160)";
  static auto const sha224 = "EMSA1(SHA-224)";
  static auto const sha256 = "EMSA1(SHA-256)";

  static auto const signatureAlgorithmEmsaMap = std::map<std::string, std::string>{
      {"SHA1withDSA(1024,160)", sha__1},
      {"SHA1withDSA", sha__1},
      {"SHA1-DSA (1024)", sha__1},
      {"SHA1-DSA (1024,160)", sha__1},
      {"DSA1024", sha__1},
      {"DSA_SHA1 (1024)", sha__1},
      {"(SHA-1, DSA 1024, Base16+11 Hexa Encoded (so stored as Ascii alphanumeric string), without ASN1)", sha__1},
      //
      {"SHA224withDSA", sha224},
      //
      {"SHA256withDSA(2048,256)", sha256},
      {"SHA256withECDSA-P256", sha256},
  };

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

  std::string UicCertificate::getEmsa(std::string const &algorithm)
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
      return sha__1;
    }
    if (std::regex_search(algorithm, sha256Pattern))
    {
      return sha256;
    }
    if (std::regex_search(algorithm, sha224Pattern))
    {
      return sha224;
    }

    throw std::runtime_error("No matching emsa value found for signatureAlgorithm: " + algorithm);
  }

  std::string UicCertificate::createMapKey(std::string const &ricsCode, std::string const &keyId)
  {
    auto stream = std::ostringstream{};
    stream << getNormalizedCode(ricsCode) << getNormalizedId(keyId);
    return stream.str();
  }

  std::string UicCertificate::getMapKey() const
  {
    auto stream = std::ostringstream{};
    stream << this->ricsCode << this->keyId;
    return stream.str();
  }

  std::string UicCertificate::toString() const
  {
    auto stream = std::ostringstream{};
    stream << "RicsCode: " << ricsCode << ", "
           << "KeyId: " << keyId << ", "
           << "Issuer: " << issuer << ", "
           << "Algorithm: " << algorithm << ", "
           << "EMSA: " << emsa;
    return stream.str();
  }

  Botan::Public_Key const &UicCertificate::getPublicKey() const
  {
    if (publicKey)
    {
      return *publicKey;
    }

    auto errors = std::vector<std::string>{};
    auto const publicKeyBytes = Botan::base64_decode(publicKey64.data(), publicKey64.size());
    try
    {
      auto dataSource = Botan::DataSource_Memory(publicKeyBytes);
      auto certificate = Botan::X509_Certificate(dataSource);
      publicKey = certificate.load_subject_public_key();
      return *publicKey;
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

      publicKey = std::make_unique<Botan::DSA_PublicKey>(algorithmIdent, keyBits);
      return *publicKey;
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

  bool UicCertificate::verify(std::vector<std::uint8_t> const &message, std::vector<std::uint8_t> const &signature) const
  {
    auto verifier = Botan::PK_Verifier(getPublicKey(), emsa, Botan::Signature_Format::IEEE_1363);

    // auto certificate = Botan::X509_Certificate(signature);
    // certificate.load_data(Botan::DataSource_Memory(message));
    // certificate.check_signature(getPublicKey());

    return verifier.verify_message(message, signature);
  }
}
