
#include "../include/Interpreter.h"

#include "lib/interpreter/detail/common/include/Context.h"

#include "lib/interpreter/detail/uic918/include/Uic918Interpreter.h"
#include "lib/interpreter/detail/vdv/include/VDVInterpreter.h"
#include "lib/interpreter/detail/sbb/include/SBBInterpreter.h"

#include "lib/interpreter/detail/uic918/include/Utility.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logger.h"
#include "lib/utility/include/Logging.h"

#include <ios>
#include <sstream>

namespace uic918::api
{
  struct Internal : public Interpreter
  {
    ::utility::Logger logger;
    std::unique_ptr<detail::Interpreter> const uicInterpreter;
    std::unique_ptr<detail::Interpreter> const vdvInterpreter;
    std::unique_ptr<detail::Interpreter> const sbbInterpreter;
    std::map<detail::Interpreter::TypeIdType, detail::Interpreter *const> interpreterMap;

    Internal(infrastructure::Context &c, std::optional<SignatureVerifier const *> signatureChecker)
        : logger(CREATE_LOGGER(c.getLoggerFactory())),
          uicInterpreter(signatureChecker
                             ? std::make_unique<detail::Uic918Interpreter>(c.getLoggerFactory(), **signatureChecker)
                             : std::make_unique<detail::Uic918Interpreter>(c.getLoggerFactory())),
          vdvInterpreter(std::make_unique<detail::VDVInterpreter>(c.getLoggerFactory())),
          sbbInterpreter(std::make_unique<detail::SBBInterpreter>(c.getLoggerFactory())),
          interpreterMap({
              {detail::Uic918Interpreter::getTypeId(), uicInterpreter.get()},
              {detail::VDVInterpreter::getTypeId(), vdvInterpreter.get()},
              {detail::SBBInterpreter::getTypeId(), sbbInterpreter.get()},
          })
    {
    }

    detail::Context interpret(detail::Context &&context) const
    {
      if (context.getRemainingSize() < 3)
      {
        LOG_WARN(logger) << "Unable to read message type, less than 3 bytes available";
        return std::move(context);
      }

      /* TODO We should peek the first bytes instead of consuming them
       */
      auto const typeId = detail::utility::getBytes(context.getPosition(), 3);
      auto const interpreter = interpreterMap.find(typeId);
      if (interpreter == interpreterMap.end())
      {
        std::stringstream os;
        os << "0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)typeId[0] << (int)typeId[1] << (int)typeId[2];
        LOG_WARN(logger) << "Unknown message type: " << os.str();
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
      return interpret(detail::Context(input, origin)).getJson(indent);
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
