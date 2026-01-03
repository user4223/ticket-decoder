// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/infrastructure/include/Logger.h"

#include "CertificateProvider.h"

#include <memory>

namespace interpreter::api
{
  class SignatureVerifier;
}

namespace interpreter::detail::vdv
{
  class VDVInterpreter : public common::Interpreter
  {
    infrastructure::Logger logger;
    std::unique_ptr<CertificateProvider> certificateProvider;

  public:
    static TypeIdType getTypeId();

    VDVInterpreter(infrastructure::LoggerFactory &loggerFactory, api::SignatureVerifier const &signatureChecker);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
