// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/api/include/CertificateProvider.h"
#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/infrastructure/include/Logger.h"

#include "MessageDecoder.h"

#include <memory>

namespace interpreter::detail::vdv
{
  class VDVInterpreter : public common::Interpreter
  {
    infrastructure::Logger logger;
    api::CertificateProvider &certificateProvider;
    std::unique_ptr<MessageDecoder> messageDecoder;

  public:
    VDVInterpreter(infrastructure::LoggerFactory &loggerFactory, api::CertificateProvider &certificateProvider);

    virtual bool canInterpret(common::Context const &context) const override;

    virtual common::Context interpret(common::Context &&context) override;
  };
}
