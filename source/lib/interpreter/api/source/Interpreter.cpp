// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Interpreter.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/interpreter/detail/uic918/include/Uic918Interpreter.h"
#include "lib/interpreter/detail/vdv/include/VDVInterpreter.h"
#include "lib/interpreter/detail/sbb/include/SBBInterpreter.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/infrastructure/include/Logger.h"
#include "lib/infrastructure/include/Logging.h"

namespace interpreter::api
{
  struct Internal : public Interpreter
  {
    infrastructure::Logger logger;
    std::map<detail::common::Interpreter::TypeIdType, std::unique_ptr<detail::common::Interpreter>> interpreterMap;

    template <typename T>
    static decltype(interpreterMap)::value_type create(auto &loggerFactory, auto const &signatureChecker)
    {
      return std::make_pair(T::getTypeId(), decltype(interpreterMap)::mapped_type{new T(loggerFactory, signatureChecker)});
    }

    Internal(infrastructure::Context &c, SignatureVerifier const &signatureChecker)
        : logger(CREATE_LOGGER(c.getLoggerFactory())),
          interpreterMap()
    {
#ifdef WITH_UIC_INTERPRETER
      interpreterMap.emplace(create<detail::uic::Uic918Interpreter>(c.getLoggerFactory(), signatureChecker));
#endif
#ifdef WITH_VDV_INTERPRETER
      interpreterMap.emplace(create<detail::vdv::VDVInterpreter>(c.getLoggerFactory(), signatureChecker));
#endif
#ifdef WITH_SBB_INTERPRETER
      interpreterMap.emplace(create<detail::sbb::SBBInterpreter>(c.getLoggerFactory(), signatureChecker));
#endif
    }

    detail::common::Context interpret(detail::common::Context &&context) const
    {
      if (context.getRemainingSize() < 3)
      {
        LOG_WARN(logger) << "Unable to read message type, less than 3 bytes available";
        return std::move(context);
      }

      auto const typeId = context.peekBytes(3);
      auto const interpreter = interpreterMap.find(detail::common::Interpreter::TypeIdType(typeId.begin(), typeId.end()));
      if (interpreter == interpreterMap.end())
      {
        LOG_WARN(logger) << "Unknown message type: " << detail::common::bytesToString(typeId);
        return std::move(context);
      }

      auto consumedContext = interpreter->second->interpret(std::move(context));
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

  std::unique_ptr<Interpreter> Interpreter::create(infrastructure::Context &context, SignatureVerifier const &signatureChecker)
  {
    return std::make_unique<Internal>(context, signatureChecker);
  }
}
