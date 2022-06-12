
#include "../include/Interpreter.h"

#include "../include/RecordHeader.h"
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/RecordInterpreterU_TLAY.h"
#include "../include/RecordInterpreter0080BL.h"
#include "../include/RecordInterpreter0080VU.h"
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
         { return std::make_unique<RecordInterpreter0080VU>(std::move(header)); }}};

struct ContextImpl : Context
{
  BytesType const &input;
  BytesType::const_iterator position;
  std::map<std::string, Field> output;

  BytesType::const_iterator &getPosition()
  {
    return position;
  }

  bool isEmpty()
  {
    return position == input.cend();
  }

  std::size_t getRemainingSize()
  {
    return std::distance(position, input.end());
  }

  Context &addField(std::string key, std::string value)
  {
    return addField(key, value, std::optional<std::string>{});
  }

  Context &addField(std::string key, std::string value, std::string description)
  {
    return addField(key, value, std::make_optional(description));
  }

  Context &addField(std::string key, std::string value, std::optional<std::string> description)
  {
    output.insert(std::make_tuple(key, Field{value, description}));
    return *this;
  }

  ContextImpl(BytesType const &i) : input(i), position(input.begin()), output() {}

  ContextImpl(BytesType const &i, std::map<std::string, Field> &&f) : input(i), position(input.begin()), output(std::move(f)) {}
};

std::map<std::string, Field> Interpreter::interpret(Context::BytesType const &input)
{
  auto context = ContextImpl{input};

  if (context.getRemainingSize() < 5)
  {
    return {};
  }
  auto const uniqueMessageTypeId = Utility::getAlphanumeric(context.getPosition(), 3);
  auto const messageTypeVersion = Utility::getAlphanumeric(context.getPosition(), 2);
  if (uniqueMessageTypeId.compare("#UT") != 0 || messageTypeVersion.compare("01") != 0)
  {
    return {};
  }
  context.addField("uniqueMessageTypeId", uniqueMessageTypeId);
  context.addField("messageTypeVersion", messageTypeVersion);
  context.addField("companyCode", Utility::getAlphanumeric(context.getPosition(), 4));
  context.addField("signatureKeyId", Utility::getAlphanumeric(context.getPosition(), 5));

  auto const signature = Utility::getBytes(context.getPosition(), 50);
  auto const messageLength = std::stoi(Utility::getAlphanumeric(context.getPosition(), 4));
  context.addField("compressedMessageLength", std::to_string(messageLength));
  if (messageLength < 0 || messageLength > context.getRemainingSize())
  {
    throw std::runtime_error("compressedMessageLength out of range: " + std::to_string(messageLength));
  }
  auto const compressedMessage = Utility::getBytes(context.getPosition(), messageLength);
  if (!context.isEmpty())
  {
    throw new std::runtime_error("Unconsumed bytes in payload");
  }

  // TODO Create hash value for compressed message and compare with signature

  auto const uncompressedMessage = Deflator::deflate(compressedMessage);
  context.addField("uncompressedMessageLength", std::to_string(uncompressedMessage.size()));

  auto messageContext = ContextImpl{uncompressedMessage, std::move(context.output)};
  auto recordIds = std::string{};
  while (!messageContext.isEmpty())
  {
    auto header = RecordHeader{messageContext};
    recordIds += recordIds.empty() ? header.recordId : "," + header.recordId;

    auto entry = recordInterpreterMap.find(header.recordId);
    if (entry != recordInterpreterMap.end())
    {
      entry->second(std::move(header))->interpret(messageContext);
    }
  }
  messageContext.addField("recordIds", recordIds);

  if (!messageContext.isEmpty())
  {
    throw new std::runtime_error("Unconsumed bytes in message");
  }

  return std::move(messageContext.output);
}
