
#include "../include/Uic918Interpreter.h"

#include "../include/RecordHeader.h"
#include "../include/RecordU_HEAD.h"
#include "../include/RecordU_TLAY.h"
#include "../include/Record0080BL.h"
#include "../include/Record0080VU.h"
#include "../include/RecordU_FLEX.h"
#include "../include/Utility.h"
#include "../include/Deflator.h"
#include "../include/Field.h"

#include "lib/uic918/api/include/Record.h"

#include "lib/utility/include/Logging.h"

#include <stdexcept>
#include <memory>
#include <functional>
#include <map>
#include <optional>

namespace uic918::detail
{
  static const std::map<std::string, std::function<std::unique_ptr<Interpreter>(::utility::LoggerFactory &loggerFactory, RecordHeader &&)>> recordInterpreterMap =
      {
          {"U_HEAD", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<RecordU_HEAD>(loggerFactory, std::move(header)); }},
          {"U_TLAY", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<RecordU_TLAY>(loggerFactory, std::move(header)); }},
          {"0080BL", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<Record0080BL>(loggerFactory, std::move(header)); }},
          {"0080VU", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<Record0080VU>(loggerFactory, std::move(header)); }},
          {"U_FLEX", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<RecordU_FLEX>(loggerFactory, std::move(header)); }}};

  Uic918Interpreter::Uic918Interpreter(::utility::LoggerFactory &lf, api::SignatureChecker const &sc)
      : loggerFactory(lf), signatureChecker(sc), logger(CREATE_LOGGER(lf)), messageContext()
  {
  }

  Context Uic918Interpreter::interpret(Context &&context)
  {
    if (context.getRemainingSize() < 5)
    {
      LOG_WARN(logger) << "Unable to read message type and version, less than 5 bytes available";
      return std::move(context);
    }
    auto const uniqueMessageTypeId = utility::getAlphanumeric(context.getPosition(), 3);
    if (uniqueMessageTypeId.compare("#UT") != 0)
    {
      LOG_WARN(logger) << "Unknown message type: " << uniqueMessageTypeId;
      return std::move(context);
    }
    auto const messageTypeVersion = utility::getAlphanumeric(context.getPosition(), 2);
    auto const version = std::stoi(messageTypeVersion);
    // Might be "OTI" as well
    if (version != 1 && version != 2)
    {
      LOG_WARN(logger) << "Unsupported message version: " << messageTypeVersion;
      return std::move(context);
    }
    context.addField("raw", context.getBase64Encoded());
    context.addField("uniqueMessageTypeId", uniqueMessageTypeId);
    context.addField("messageTypeVersion", messageTypeVersion);
    auto const ricsCode = utility::getAlphanumeric(context.getPosition(), 4);
    context.addField("companyCode", ricsCode);
    auto const keyId = utility::getAlphanumeric(context.getPosition(), 5);
    context.addField("signatureKeyId", keyId);

    auto const signatureLength = version == 2 ? 64 : 50;
    auto const signature = utility::getBytes(context.getPosition(), signatureLength);
    auto const consumed = context.getConsumedSize();
    auto const messageLengthString = utility::getAlphanumeric(context.getPosition(), 4);
    auto const messageLength = std::stoi(messageLengthString);
    context.addField("compressedMessageLength", std::to_string(messageLength));
    if (messageLength < 0 || messageLength > context.getRemainingSize())
    {
      throw std::runtime_error("compressedMessageLength out of range: " + std::to_string(messageLength));
    }
    auto const compressedMessage = utility::getBytes(context.getPosition(), messageLength);
    if (!context.isEmpty())
    {
      throw std::runtime_error("Unconsumed bytes in payload");
    }

    auto const validationResult = signatureChecker.check(ricsCode, keyId, compressedMessage, signature);
    if (validationResult == api::SignatureChecker::Result::KeyNotFound)
    {
      LOG_DEBUG(logger) << "No certificate available to validate: " << ricsCode << " / " << keyId;
    }
    context.addField("validated", validationResult == api::SignatureChecker::Result::Successful ? "true" : "false");

    auto const uncompressedMessage = deflate(compressedMessage);
    context.addField("uncompressedMessageLength", std::to_string(uncompressedMessage.size()));

    auto messageContext = Context(uncompressedMessage, std::move(context.output));
    while (!messageContext.isEmpty())
    {
      auto header = RecordHeader{messageContext};
      auto const entry = recordInterpreterMap.find(header.recordId);
      if (entry != recordInterpreterMap.end())
      {
        auto &factory = entry->second;
        auto interpreter = factory(loggerFactory, std::move(header));
        messageContext = interpreter->interpret(std::move(messageContext));
      }
      else // skip block
      {
        auto position = messageContext.getPosition();
        auto const remaining = header.getRemaining(position);
        utility::getBytes(position, remaining);

        LOG_WARN(logger) << "Ignoring " << remaining << " bytes containing unknown record: " << header.toString();
      }
    }

    if (!messageContext.isEmpty())
    {
      throw std::runtime_error("Unconsumed bytes in message");
    }

    return std::move(messageContext);
  }

  /* TODO Separate unification of different uic918 dialects into single output format somewhere different
   */
  struct DefaultTicket
  {
    std::map<std::string, Field> fields;

    DefaultTicket(std::map<std::string, Field> &&f) : fields(std::move(f)) {}

    std::optional<std::string> toOptional(std::string const &key)
    {
      auto const entry = fields.find(key);
      return entry == fields.end()
                 ? std::optional<std::string>()
                 : std::make_optional(entry->second.value);
    }

    virtual std::optional<std::string> getUniqueId()
    {
      return toOptional("U_HEAD.uniqueTicketKey");
    }
  };

  struct DBTicket : DefaultTicket
  {
    using DefaultTicket::DefaultTicket;

    virtual std::optional<std::string> getGivenName()
    {
      auto field = toOptional("0080BL.fieldS028");
      if (field)
      {
        auto const delimiter = field->find('#');
        if (delimiter)
        {
          return std::make_optional(field->substr(0, delimiter));
        }
      }
      return field;
    }

    virtual std::optional<std::string> getFamilyName()
    {
      auto field = toOptional("0080BL.fieldS028");
      if (field)
      {
        auto const delimiter = field->find('#');
        if (delimiter)
        {
          return std::make_optional(field->substr(delimiter + 1, field->size()));
        }
      }
      return field;
    }
  };

  //  std::unique_ptr<DefaultTicket> interpretTicket(std::vector<std::uint8_t> const &input)
  //  {
  //    auto fields = Interpreter::interpret(input)->getFields();
  //    if (fields.empty())
  //    {
  //      return {};
  //    }
  //    auto const companyCode = fields.at("companyCode").value;
  //    if (companyCode != "0080" && companyCode != "1080")
  //    {
  //      throw std::runtime_error("Unsupported company code: " + companyCode);
  //    }
  //    return std::make_unique<DBTicket>(std::move(fields));
  //  }
}
