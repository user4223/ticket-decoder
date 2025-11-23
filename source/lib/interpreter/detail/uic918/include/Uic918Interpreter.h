// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/utility/include/LoggingFwd.h"

#include "lib/interpreter/api/include/SignatureVerifier.h"

#include <memory>

namespace interpreter::detail::uic
{
  class Uic918Interpreter : public common::Interpreter
  {
    ::utility::LoggerFactory &loggerFactory;
    ::utility::Logger logger;
    api::SignatureVerifier const *const signatureChecker;
    std::unique_ptr<common::Context> messageContext;

  public:
    static TypeIdType getTypeId();

    Uic918Interpreter(::utility::LoggerFactory &loggerFactory, api::SignatureVerifier const &signatureChecker);

    Uic918Interpreter(::utility::LoggerFactory &loggerFactory);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
