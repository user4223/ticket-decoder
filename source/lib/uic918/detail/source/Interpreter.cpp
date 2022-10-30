
#include "../include/Interpreter.h"

#include "../api/include/Record.h"

#include "../include/RecordHeader.h"
#include "../include/RecordU_HEAD.h"
#include "../include/RecordU_TLAY.h"
#include "../include/Record0080BL.h"
#include "../include/Record0080VU.h"
#include "../include/RecordU_FLEX.h"
#include "../include/Utility.h"
#include "../include/Deflator.h"
#include "../include/Field.h"

#include "lib/utility/include/JsonBuilder.h"
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

  struct ContextImpl : Context
  {
    BytesType const &input;
    BytesType::const_iterator position;
    std::map<std::string, Field> output;
    std::map<std::string, api::Record> records;

    BytesType::const_iterator &getPosition() override
    {
      return position;
    }

    bool isEmpty() override
    {
      return position == input.cend();
    }

    std::size_t getRemainingSize() override
    {
      return std::distance(position, input.end());
    }

    std::map<std::string, Field> const &getFields() override
    {
      return output;
    }

    std::optional<Field> getField(std::string key) override
    {
      auto const entry = output.find(key);
      return entry == output.end() ? std::optional<Field>{} : entry->second;
    }

    Context &setField(std::string key, Field &&field) override
    {
      output[key] = std::move(field);
      return *this;
    }

    Context &addField(std::string key, std::string value) override
    {
      return addField(key, value, std::optional<std::string>{});
    }

    Context &addField(std::string key, std::string value, std::string description) override
    {
      return addField(key, value, std::make_optional(description));
    }

    Context &addField(std::string key, std::string value, std::optional<std::string> description) override
    {
      return setField(key, Field{value, description});
    }

    std::optional<std::string> getJson() override
    {
      if (records.empty())
      {
        return std::nullopt;
      }

      using json = nlohmann::json;
      auto result = json::object();
      result["records"] = std::reduce(records.begin(), records.end(), json::object(),
                                      [](auto &&result, auto const &record)
                                      {
                                        result[record.first] = json::parse(record.second.getJson());
                                        return std::move(result);
                                      });

      return std::make_optional(std::move(result.dump()));
    }

    Context &addRecord(api::Record &&record) override
    {
      auto const id = record.getId();
      records.insert(std::make_pair(id, std::move(record)));
      return *this;
    }

    std::optional<api::Record> tryGetRecord(std::string recordKey) override
    {
      auto const record = records.find(recordKey);
      return record == records.end() ? std::nullopt : std::make_optional(record->second);
    }

    api::Record getRecord(std::string recordKey) override
    {
      auto const record = tryGetRecord(recordKey);
      if (!record)
      {
        throw std::runtime_error("Record not found: " + recordKey);
      }
      return *record;
    }

    std::map<std::string, api::Record> const &getRecords() override
    {
      return records;
    }

    ContextImpl(BytesType const &i)
        : input(i),
          position(input.begin()),
          output() {}

    ContextImpl(BytesType const &i, std::map<std::string, Field> &&f)
        : input(i),
          position(input.begin()),
          output(std::move(f)) {}
  };

  std::unique_ptr<Context> Interpreter::interpret(::utility::LoggerFactory &loggerFactory, Context::BytesType const &input)
  {
    auto logger = CREATE_LOGGER(loggerFactory);
    auto context = std::make_unique<ContextImpl>(input);

    if (context->getRemainingSize() < 5)
    {
      LOG_WARN(logger) << "Unable to read message type and version, less than 5 bytes available";
      return std::move(context);
    }
    auto const uniqueMessageTypeId = utility::getAlphanumeric(context->getPosition(), 3);
    if (uniqueMessageTypeId.compare("#UT") != 0)
    {
      LOG_WARN(logger) << "Unknown message type: " << uniqueMessageTypeId;
      return context;
    }
    auto const messageTypeVersion = utility::getAlphanumeric(context->getPosition(), 2);
    auto const version = std::stoi(messageTypeVersion);
    // Might be "OTI" as well
    if (version != 1 && version != 2)
    {
      LOG_WARN(logger) << "Unsupported message version: " << messageTypeVersion;
      return context;
    }
    context->addField("uniqueMessageTypeId", uniqueMessageTypeId);
    context->addField("messageTypeVersion", messageTypeVersion);
    context->addField("companyCode", utility::getAlphanumeric(context->getPosition(), 4));
    context->addField("signatureKeyId", utility::getAlphanumeric(context->getPosition(), 5));

    auto const signature = utility::getBytes(context->getPosition(), version == 2 ? 64 : 50);
    auto const messageLength = std::stoi(utility::getAlphanumeric(context->getPosition(), 4));
    context->addField("compressedMessageLength", std::to_string(messageLength));
    if (messageLength < 0 || messageLength > context->getRemainingSize())
    {
      throw std::runtime_error("compressedMessageLength out of range: " + std::to_string(messageLength));
    }
    auto const compressedMessage = utility::getBytes(context->getPosition(), messageLength);
    if (!context->isEmpty())
    {
      throw std::runtime_error("Unconsumed bytes in payload");
    }

    // TODO Create hash value for compressed message and compare with signature

    auto const uncompressedMessage = deflate(compressedMessage);
    context->addField("uncompressedMessageLength", std::to_string(uncompressedMessage.size()));

    auto messageContext = std::make_unique<ContextImpl>(uncompressedMessage, std::move(context->output));
    while (!messageContext->isEmpty())
    {
      auto header = RecordHeader{*messageContext};
      auto const entry = recordInterpreterMap.find(header.recordId);
      if (entry != recordInterpreterMap.end())
      {
        auto &factory = entry->second;
        auto interpreter = factory(loggerFactory, std::move(header));
        interpreter->interpret(*messageContext);
      }
      else // skip block
      {
        auto position = messageContext->getPosition();
        auto const remaining = header.getRemaining(position);
        utility::getBytes(position, remaining);

        LOG_WARN(logger) << "Ignoring unknown record, skipped bytes: " << remaining;
      }
    }

    if (!messageContext->isEmpty())
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

  //  std::unique_ptr<DefaultTicket> interpretTicket(Context::BytesType const &input)
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
