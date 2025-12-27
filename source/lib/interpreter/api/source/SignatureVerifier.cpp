// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/SignatureVerifier.h"
#include "../include/NopSignatureVerifier.h"

#ifdef WITH_SIGNATURE_VERIFIER
#include "lib/interpreter/detail/verifier/include/BotanSignatureVerifier.h"
#endif

#include "lib/infrastructure/include/Context.h"
#include "lib/infrastructure/include/Logging.h"

namespace interpreter::api
{
  std::unique_ptr<SignatureVerifier> SignatureVerifier::create(
      infrastructure::Context &context,
      std::filesystem::path const &uicSignatureXml)
  {
#ifdef WITH_SIGNATURE_VERIFIER
    return std::make_unique<detail::verifier::BotanSignatureVerifier>(context, uicSignatureXml);
#else
    return createDummy(context);
#endif
  }

  std::unique_ptr<SignatureVerifier> SignatureVerifier::createDummy(infrastructure::Context &context)
  {
    return std::make_unique<NopSignatureVerifier>(context);
  }
}
