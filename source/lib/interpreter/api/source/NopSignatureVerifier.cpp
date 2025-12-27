// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/NopSignatureVerifier.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::api
{
  NopSignatureVerifier::NopSignatureVerifier(infrastructure::Context &context)
      : logger(CREATE_LOGGER(context.getLoggerFactory()))
  {
    LOG_WARN(logger) << "Using dummy signature checker";
  }

  NopSignatureVerifier::Result NopSignatureVerifier::check(
      std::string const &ricsCode, std::string const &keyId,
      std::span<std::uint8_t const> message,
      std::span<std::uint8_t const> signature) const
  {
    return Result::KeyNotFound;
  }

  std::unique_ptr<SignatureVerifier> NopSignatureVerifier::create(infrastructure::Context &context)
  {
    return std::make_unique<NopSignatureVerifier>(context);
  }
}
