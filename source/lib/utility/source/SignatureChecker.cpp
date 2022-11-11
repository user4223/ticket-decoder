#include "../include/SignatureChecker.h"

#include "lib/utility/include/Logging.h"

#include <pugixml.hpp>

#include <botan/pubkey.h>
#include <botan/ber_dec.h>
#include <botan/base64.h>
#include <botan/x509_ca.h>
#include <botan/x509_key.h>
#include <botan/dsa.h>

#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <regex>
#include <numeric>

namespace utility
{
  static auto const digitsOnly = std::regex("\\d+");
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

  struct Key
  {
    std::string const ricsCode;
    unsigned long const keyId;
    std::string const issuer;
    std::string const algorithm;
    std::string const emsa;
    std::string const publicKey64;
    mutable std::unique_ptr<Botan::Public_Key> publicKey; // lazy load from base64 string

    Key(pugi::xml_node const &xml) : ricsCode(getNormalizedCode(xml.child_value("issuerCode"))),
                                     keyId(getNormalizedId(xml.child_value("id"))),
                                     issuer(xml.child_value("issuerName")),
                                     algorithm(xml.child_value("signatureAlgorithm")),
                                     emsa(getEmsa(algorithm)),
                                     publicKey64(xml.child_value("publicKey")),
                                     publicKey()
    {
    }

    static std::string getNormalizedCode(std::string const &ricsCode)
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

    static unsigned long getNormalizedId(std::string const &keyId)
    {
      if (keyId.empty())
      {
        throw std::runtime_error("Key id empty");
      }
      if (!std::regex_match(keyId, digitsOnly))
      {
        throw std::runtime_error("Key id invalid, expecting digits only: " + keyId);
      }

      return std::stoul(keyId);
    }

    static std::string getEmsa(std::string const &algorithm)
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

    static std::string createMapKey(std::string const &c, std::string const &i)
    {
      auto stream = std::ostringstream{};
      stream << getNormalizedCode(c) << getNormalizedId(i);
      return stream.str();
    }

    std::string getMapKey() const
    {
      auto stream = std::ostringstream{};
      stream << this->ricsCode << this->keyId;
      return stream.str();
    }

    std::string toString() const
    {
      auto stream = std::ostringstream{};
      stream << "RicsCode: " << ricsCode << ", "
             << "KeyId: " << keyId << ", "
             << "Issuer: " << issuer << ", "
             << "Algorithm: " << algorithm << ", "
             << "EMSA: " << emsa;
      return stream.str();
    }

    Botan::Public_Key const &loadPublicKey() const
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

    SignatureChecker::Result verify(std::vector<std::uint8_t> const &message, std::vector<std::uint8_t> const &signature) const
    {
      auto verifier = Botan::PK_Verifier(loadPublicKey(), emsa, Botan::Signature_Format::IEEE_1363);
      auto const verified = verifier.verify_message(message, signature);
      return verified ? SignatureChecker::Result::Successful : SignatureChecker::Result::Failed;
    }
  };

  struct UicSignatureChecker : public SignatureChecker
  {
    utility::Logger logger;
    std::map<std::string, Key const> keys;

    UicSignatureChecker(LoggerFactory &loggerFactory, std::filesystem::path const &uicSignatureXml)
        : logger(CREATE_LOGGER(loggerFactory))
    {
      if (!std::filesystem::exists(uicSignatureXml) || !std::filesystem::is_regular_file(uicSignatureXml))
      {
        LOG_WARN(logger) << "UIC signature file not found or not a regular file: " << uicSignatureXml;
        return;
      }

      auto doc = pugi::xml_document{};
      auto const result = doc.load_file(uicSignatureXml.c_str());
      if (!result)
      {
        LOG_WARN(logger) << "Loading UIC signature file failed with: " << result.description();
        return;
      }

      for (auto const entry : doc.child("keys").children("key"))
      {
        auto key = Key(entry);
        try
        {
          key.loadPublicKey(); // We check here if we can load this key properly

          auto const mapKey = key.getMapKey();
          keys.emplace(std::make_pair(mapKey, std::move(key)));

          LOG_DEBUG(logger) << "Successfully loaded: " << key.toString();
        }
        catch (std::exception const &e)
        {
          LOG_WARN(logger) << "Loading failed: " << key.toString() << " with: " << e.what();
        }
      }

      LOG_INFO(logger) << "Number of valid public keys: " << keys.size();
    }

    virtual SignatureChecker::Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &signature,
        std::vector<std::uint8_t> const &message) const override
    {
      auto const entryId = Key::createMapKey(ricsCode, keyId);
      auto const entry = keys.find(entryId);
      if (entry == keys.end())
      {
        return Result::KeyNotFound;
      }
      return entry->second.verify(signature, message);
    }
  };

  std::unique_ptr<SignatureChecker> SignatureChecker::create(
      LoggerFactory &loggerFactory,
      std::filesystem::path const &uicSignatureXml)
  {
    return std::make_unique<UicSignatureChecker>(loggerFactory, uicSignatureXml);
  }
}
