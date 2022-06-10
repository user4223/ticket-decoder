
#include "../include/Interpreter.h"

#include "../include/RecordHeader.h"
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/RecordInterpreterU_TLAY.h"
#include "../include/RecordInterpreter0080BL.h"
#include "../include/RecordInterpreter0080VU.h"
#include "../include/Utility.h"
#include "../include/Deflator.h"

#include <stdexcept>
#include <memory>
#include <functional>

static const std::map<std::string, std::function<std::unique_ptr<Interpreter>(RecordHeader &&)>> interpreterMap =
    {
        {"U_HEAD", [](auto &&header)
         { return std::make_unique<RecordInterpreterU_HEAD>(std::move(header)); }},
        {"U_TLAY", [](auto &&header)
         { return std::make_unique<RecordInterpreterU_TLAY>(std::move(header)); }},
        {"0080BL", [](auto &&header)
         { return std::make_unique<RecordInterpreter0080BL>(std::move(header)); }},
        {"0080VU", [](auto &&header)
         { return std::make_unique<RecordInterpreter0080VU>(std::move(header)); }},
};

struct TLBInterpreter : Interpreter
{
  Context &interpret(Context &context)
  {
    if (context.input.size() < 5)
    {
      return context;
    }
    auto position = Interpreter::BytesType::const_iterator(context.input.begin());
    auto const uniqueMessageTypeId = Utility::getAlphanumeric(position, 3);
    auto const messageTypeVersion = Utility::getAlphanumeric(position, 2);
    if (uniqueMessageTypeId.compare("#UT") != 0 || messageTypeVersion.compare("01") != 0)
    {
      return context;
    }
    context.output.insert(std::make_pair("uniqueMessageTypeId", uniqueMessageTypeId));
    context.output.insert(std::make_pair("messageTypeVersion", messageTypeVersion));
    context.output.insert(std::make_pair("companyCode", Utility::getAlphanumeric(position, 4)));
    context.output.insert(std::make_pair("signatureKeyId", Utility::getAlphanumeric(position, 5)));

    context.signature = Utility::getBytes(position, 50);
    auto const compressedMessageLength = Utility::getAlphanumeric(position, 4);
    context.output.insert(std::make_pair("compressedMessageLength", compressedMessageLength));
    auto const length = std::stoi(compressedMessageLength);
    if (length < 0 || length > std::distance(position, context.input.end()))
    {
      throw std::runtime_error("compressedMessageLength out of range: " + compressedMessageLength);
    }
    context.compressedMessage = Utility::getBytes(position, length);
    // TODO Create hash value for compressed message and compare with signature

    context.uncompressedMessage = Deflator::deflate(context.compressedMessage);
    context.position = context.uncompressedMessage.begin();

    while (context.position != context.uncompressedMessage.end())
    {
      auto header = RecordHeader(context.position);
      context.recordIds.push_back(header.recordId);

      auto entry = interpreterMap.find(header.recordId);
      if (entry != interpreterMap.end())
      {
        entry->second(std::move(header))->interpret(context);
      }

      context.position = header.startPosition + header.recordLength;
    }

    return context;
  }
};

Interpreter::Context Interpreter::interpret(BytesType const &input)
{
  auto context = Context{input};
  return TLBInterpreter().interpret(context);
}
