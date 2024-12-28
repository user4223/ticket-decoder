
#include "../include/Interpreter.h"

#include "lib/uic918/detail/include/Context.h"
#include "lib/uic918/detail/include/Uic918Interpreter.h"
#include "lib/uic918/detail/include/VDVInterpreter.h"
#include "lib/uic918/detail/include/Utility.h"

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
    std::unique_ptr<detail::Uic918Interpreter> const uicInterpreter;
    std::unique_ptr<detail::VDVInterpreter> const vdvInterpreter;
    std::map<detail::Interpreter::TypeIdType, detail::Interpreter *const> interpreterMap;

    Internal(infrastructure::Context &c, std::optional<SignatureChecker const *> signatureChecker)
        : logger(CREATE_LOGGER(c.getLoggerFactory())),
          uicInterpreter(signatureChecker
                             ? std::make_unique<detail::Uic918Interpreter>(c.getLoggerFactory(), **signatureChecker)
                             : std::make_unique<detail::Uic918Interpreter>(c.getLoggerFactory())),
          vdvInterpreter(std::make_unique<detail::VDVInterpreter>(c.getLoggerFactory())),
          interpreterMap({{detail::Uic918Interpreter::getTypeId(), reinterpret_cast<detail::Interpreter *>(uicInterpreter.get())},
                          {detail::VDVInterpreter::getTypeId(), reinterpret_cast<detail::Interpreter *>(vdvInterpreter.get())}})
    {
    }

    detail::Context interpret(detail::Context &&context) const
    {
      if (context.getRemainingSize() < 3)
      {
        LOG_WARN(logger) << "Unable to read message type, less than 3 bytes available";
        return std::move(context);
      }

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

  std::unique_ptr<Interpreter> Interpreter::create(infrastructure::Context &context, SignatureChecker const &signatureChecker)
  {
    return std::make_unique<Internal>(context, std::make_optional(&signatureChecker));
  }

  std::unique_ptr<Interpreter> Interpreter::create(infrastructure::Context &context)
  {
    return std::make_unique<Internal>(context, std::nullopt);
  }
}
