// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Certificate.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include "lib/interpreter/api/include/SignatureVerifier.h"

#include <map>
#include <string>
#include <filesystem>

namespace interpreter::detail::verifier
{
  class BotanSignatureVerifier : public api::SignatureVerifier
  {
    ::utility::Logger logger;
    std::map<std::string, Certificate const> keys;

  public:
    BotanSignatureVerifier(infrastructure::Context &context, std::filesystem::path const &uicSignatureXml);

    virtual api::SignatureVerifier::Result check(
        std::string const &ricsCode, std::string const &keyId,
        std::span<std::uint8_t const> message,
        std::span<std::uint8_t const> signature) const override;
  };
}
