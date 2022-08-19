
#include "../include/Interpreter.h"

#include "../include/RecordHeader.h"
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/RecordInterpreterU_TLAY.h"
#include "../include/RecordInterpreter0080BL.h"
#include "../include/RecordInterpreter0080VU.h"
#include "../include/RecordInterpreterU_FLEX.h"
#include "../include/Utility.h"
#include "../include/Deflator.h"
#include "../include/Field.h"

#include <stdexcept>
#include <memory>
#include <functional>
#include <map>
#include <optional>

static const std::map<std::string, std::function<std::unique_ptr<Interpreter>(RecordHeader &&)>> recordInterpreterMap =
    {
        {"U_HEAD", [](auto &&header)
         { return std::make_unique<RecordInterpreterU_HEAD>(std::move(header)); }},
        {"U_TLAY", [](auto &&header)
         { return std::make_unique<RecordInterpreterU_TLAY>(std::move(header)); }},
        {"0080BL", [](auto &&header)
         { return std::make_unique<RecordInterpreter0080BL>(std::move(header)); }},
        {"0080VU", [](auto &&header)
         { return std::make_unique<RecordInterpreter0080VU>(std::move(header)); }},
        {"U_FLEX", [](auto &&header)
         { return std::make_unique<RecordInterpreterU_FLEX>(std::move(header)); }}};

struct ContextImpl : Context
{
  BytesType const &input;
  BytesType::const_iterator position;
  std::map<std::string, Field> output;
  std::map<std::string, std::string> records;

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

  Context &addRecord(std::string recordKey, std::string json) override
  {
    records.insert(std::make_pair(recordKey, json));
    return *this;
  }

  std::string getRecord(std::string recordKey) override
  {
    auto const record = records.find(recordKey);
    if (record == records.end())
    {
      throw std::runtime_error("Record not found: " + recordKey);
    }
    return record->second;
  }

  ContextImpl(BytesType const &i) : input(i), position(input.begin()), output()
  {
  }

  ContextImpl(BytesType const &i, std::map<std::string, Field> &&f) : input(i), position(input.begin()), output(std::move(f)) {}
};

std::map<std::string, Field> Interpreter::interpretRaw(Context::BytesType const &input)
{
  return std::move(interpret(input)->getFields());
}

std::unique_ptr<Context> Interpreter::interpret(Context::BytesType const &input)
{
  auto context = std::make_unique<ContextImpl>(input);

  if (context->getRemainingSize() < 5)
  {
    return std::move(context);
  }
  auto const uniqueMessageTypeId = Utility::getAlphanumeric(context->getPosition(), 3);
  auto const messageTypeVersion = Utility::getAlphanumeric(context->getPosition(), 2);
  auto const version = std::stoi(messageTypeVersion);
  // Might be "OTI" as well
  if (uniqueMessageTypeId.compare("#UT") != 0 || (version != 1 && version != 2))
  {
    return context;
  }
  context->addField("uniqueMessageTypeId", uniqueMessageTypeId);
  context->addField("messageTypeVersion", messageTypeVersion);
  context->addField("companyCode", Utility::getAlphanumeric(context->getPosition(), 4));
  context->addField("signatureKeyId", Utility::getAlphanumeric(context->getPosition(), 5));

  auto const signature = Utility::getBytes(context->getPosition(), version == 2 ? 64 : 50);
  auto const messageLength = std::stoi(Utility::getAlphanumeric(context->getPosition(), 4));
  context->addField("compressedMessageLength", std::to_string(messageLength));
  if (messageLength < 0 || messageLength > context->getRemainingSize())
  {
    throw std::runtime_error("compressedMessageLength out of range: " + std::to_string(messageLength));
  }
  auto const compressedMessage = Utility::getBytes(context->getPosition(), messageLength);
  if (!context->isEmpty())
  {
    throw std::runtime_error("Unconsumed bytes in payload");
  }

  // TODO Create hash value for compressed message and compare with signature

  auto const uncompressedMessage = Deflator::deflate(compressedMessage);
  context->addField("uncompressedMessageLength", std::to_string(uncompressedMessage.size()));

  auto messageContext = std::make_unique<ContextImpl>(uncompressedMessage, std::move(context->output));
  while (!messageContext->isEmpty())
  {
    auto header = RecordHeader{*messageContext};
    auto const entry = recordInterpreterMap.find(header.recordId);
    if (entry != recordInterpreterMap.end())
    {
      entry->second(std::move(header))->interpret(*messageContext);
    }
    else // skip block
    {
      Utility::getBytes(messageContext->getPosition(), header.getRemaining(messageContext->getPosition()));
    }
  }

  if (!messageContext->isEmpty())
  {
    throw std::runtime_error("Unconsumed bytes in message");
  }

  return std::move(messageContext);
}

struct DefaultTicket : Ticket
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

  virtual std::optional<std::string> getUniqueId() override
  {
    return toOptional("U_HEAD.uniqueTicketKey");
  }
};

struct DBTicket : DefaultTicket
{
  using DefaultTicket::DefaultTicket;

  virtual std::optional<std::string> getGivenName() override
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

  virtual std::optional<std::string> getFamilyName() override
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

std::unique_ptr<Ticket> Interpreter::interpretTicket(Context::BytesType const &input)
{
  auto fields = interpretRaw(input);
  if (fields.empty())
  {
    return {};
  }
  auto const companyCode = fields.at("companyCode").value;
  if (companyCode != "0080" && companyCode != "1080")
  {
    throw std::runtime_error("Unsupported company code: " + companyCode);
  }
  return std::make_unique<DBTicket>(std::move(fields));
}
