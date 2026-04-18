// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Interpreter.h"

#include "lib/interpreter/api/include/CertificateProvider.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/StringDecoder.h"

#include "lib/interpreter/detail/uic918/include/Uic918Interpreter.h"
#include "lib/interpreter/detail/vdv/include/VDVInterpreter.h"
#include "lib/interpreter/detail/sbb/include/SBBInterpreter.h"
#include "lib/interpreter/detail/misc/include/CSVInterpreter.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/infrastructure/include/Logger.h"
#include "lib/infrastructure/include/Logging.h"

#include <vector>

namespace interpreter::api
{
  struct Internal : public Interpreter
  {
    infrastructure::Logger logger;
    std::vector<std::unique_ptr<detail::common::Interpreter>> interpreterList;

    Internal(infrastructure::Context &c, SignatureVerifier const &signatureChecker, CertificateProvider &certificateProvider)
        : logger(CREATE_LOGGER(c.getLoggerFactory())),
          interpreterList()
    {
#ifdef WITH_UIC_INTERPRETER
      interpreterList.emplace_back(std::make_unique<detail::uic::Uic918Interpreter>(c.getLoggerFactory(), signatureChecker));
#endif
#ifdef WITH_VDV_INTERPRETER
      interpreterList.emplace_back(std::make_unique<detail::vdv::VDVInterpreter>(c.getLoggerFactory(), certificateProvider));
#endif
#ifdef WITH_SBB_INTERPRETER
      interpreterList.emplace_back(std::make_unique<detail::sbb::SBBInterpreter>(c.getLoggerFactory(), signatureChecker));
#endif
      interpreterList.emplace_back(std::make_unique<detail::misc::CSVInterpreter>(c.getLoggerFactory()));
    }

    detail::common::Context interpret(detail::common::Context &&context) const
    {
      auto const interpreter = std::find_if(std::begin(interpreterList), std::end(interpreterList), [&](auto const &interpreter){
        return interpreter->canInterpret(context);
      });
      if (interpreter == interpreterList.end())
      {
        LOG_WARN(logger) << "Unknown message: " << detail::common::StringDecoder::toHexString(context.peekMaximalBytes(30)) << "...";
        return std::move(context);
      }

      auto consumedContext = (*interpreter)->interpret(std::move(context));
      if (!consumedContext.isEmpty())
      {
        LOG_WARN(logger) << "Unconsumed bytes detected: " << consumedContext.getRemainingSize();
      }

      return consumedContext;
    }

    virtual std::optional<std::string> interpret(std::vector<std::uint8_t> const &input, std::string origin, int indent = -1) const override
    {
      if (input.empty())
      {
        return std::nullopt;
      }
      return interpret(detail::common::Context(input, origin)).getJson(indent);
    }
  };

  std::unique_ptr<Interpreter> Interpreter::create(infrastructure::Context &context, SignatureVerifier const &signatureChecker, CertificateProvider &certificateProvider)
  {
    return std::make_unique<Internal>(context, signatureChecker, certificateProvider);
  }
}
