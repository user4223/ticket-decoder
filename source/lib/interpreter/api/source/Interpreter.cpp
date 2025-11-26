// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Interpreter.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/interpreter/detail/uic918/include/Uic918Interpreter.h"
#include "lib/interpreter/detail/vdv/include/VDVInterpreter.h"
#include "lib/interpreter/detail/sbb/include/SBBInterpreter.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logger.h"
#include "lib/utility/include/Logging.h"

namespace interpreter::api
{
  struct Internal : public Interpreter
  {
    ::utility::Logger logger;
    std::unique_ptr<detail::common::Interpreter> const uicInterpreter;
    std::unique_ptr<detail::common::Interpreter> const vdvInterpreter;
    std::unique_ptr<detail::common::Interpreter> const sbbInterpreter;
    std::map<detail::common::Interpreter::TypeIdType, detail::common::Interpreter *const> interpreterMap;

    Internal(infrastructure::Context &c, std::optional<SignatureVerifier const *> signatureChecker)
        : logger(CREATE_LOGGER(c.getLoggerFactory())),
          uicInterpreter(signatureChecker
                             ? std::make_unique<detail::uic::Uic918Interpreter>(c.getLoggerFactory(), **signatureChecker)
                             : std::make_unique<detail::uic::Uic918Interpreter>(c.getLoggerFactory())),
          vdvInterpreter(std::make_unique<detail::vdv::VDVInterpreter>(c.getLoggerFactory())),
          sbbInterpreter(std::make_unique<detail::sbb::SBBInterpreter>(c.getLoggerFactory())),
          interpreterMap({
              {detail::uic::Uic918Interpreter::getTypeId(), uicInterpreter.get()},
              {detail::vdv::VDVInterpreter::getTypeId(), vdvInterpreter.get()},
              {detail::sbb::SBBInterpreter::getTypeId(), sbbInterpreter.get()},
          })
    {
    }

    detail::common::Context interpret(detail::common::Context &&context) const
    {
      if (context.getRemainingSize() < 3)
      {
        LOG_WARN(logger) << "Unable to read message type, less than 3 bytes available";
        return std::move(context);
      }

      auto const typeId = context.peekBytes(3);
      auto const interpreter = interpreterMap.find(typeId);
      if (interpreter == interpreterMap.end())
      {
        LOG_WARN(logger) << "Unknown message type: " << detail::common::bytesToString(typeId);
        return std::move(context);
      }

      return interpreter->second->interpret(std::move(context));
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

  std::unique_ptr<Interpreter> Interpreter::create(infrastructure::Context &context, SignatureVerifier const &signatureChecker)
  {
    return std::make_unique<Internal>(context, std::make_optional(&signatureChecker));
  }

  std::unique_ptr<Interpreter> Interpreter::create(infrastructure::Context &context)
  {
    return std::make_unique<Internal>(context, std::nullopt);
  }
}
