// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "SignatureVerifier.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

namespace interpreter::api
{
  /* Dummy implementation returning always KeyNotFound
   */
  class NopSignatureVerifier : public SignatureVerifier
  {
    ::utility::Logger logger;

  public:
    NopSignatureVerifier(infrastructure::Context &context);

    static std::unique_ptr<SignatureVerifier> create(infrastructure::Context &context);

    virtual Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::vector<std::uint8_t> const &message,
        std::vector<std::uint8_t> const &signature) const override;
  };
}
