
#pragma once

#include "lib/utility/include/SignatureChecker.h"

#include <string>
#include <vector>

namespace support
{
  std::unique_ptr<utility::SignatureChecker> getSignatureChecker();

  std::vector<std::uint8_t> getData(std::string fileName);
}
