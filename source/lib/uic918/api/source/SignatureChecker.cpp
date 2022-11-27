#include "../include/SignatureChecker.h"

#include "lib/uic918/detail/include/UicSignatureChecker.h"

namespace uic918::api
{

  std::unique_ptr<SignatureChecker> SignatureChecker::create(
      ::utility::LoggerFactory &loggerFactory,
      std::filesystem::path const &uicSignatureXml)
  {
    return std::make_unique<detail::UicSignatureChecker>(loggerFactory, uicSignatureXml);
  }
}
