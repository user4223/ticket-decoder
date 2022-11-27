
#pragma once

#include "lib/uic918/api/include/SignatureChecker.h"

#include <string>
#include <vector>

namespace support
{
  std::unique_ptr<uic918::api::SignatureChecker> getSignatureChecker();

  std::vector<std::uint8_t> getData(std::string fileName);
}
