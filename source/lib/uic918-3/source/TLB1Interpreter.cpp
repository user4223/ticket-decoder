
#include "../include/TLB1Interpreter.h"
#include "../include/Utility.h"
#include "../include/Deflator.h"

#include <stdexcept>

Interpreter::Context TLB1Interpreter::interpret(Context &&context)
{
  if (context.input.size() < 5)
  {
    return std::move(context);
  }

  auto const uniqueMessageTypeId = Utility::getAlphanumeric(context.position, 3);
  auto const messageTypeVersion = Utility::getAlphanumeric(context.position, 2);
  if (uniqueMessageTypeId.compare("#UT") != 0 || messageTypeVersion.compare("01") != 0)
  {
    return std::move(context);
  }
  context.output.insert(std::make_pair("uniqueMessageTypeId", uniqueMessageTypeId));
  context.output.insert(std::make_pair("messageTypeVersion", messageTypeVersion));
  context.output.insert(std::make_pair("companyCode", Utility::getAlphanumeric(context.position, 4)));
  context.output.insert(std::make_pair("signatureKeyId", Utility::getAlphanumeric(context.position, 5)));
  auto const signature = std::vector<std::uint8_t>{context.position, context.position += 50};
  auto const compressedMessageLength = Utility::getAlphanumeric(context.position, 4);
  context.output.insert(std::make_pair("compressedMessageLength", compressedMessageLength));
  auto const length = std::stoi(compressedMessageLength);
  if (length < 0 || length > std::distance(context.position, context.input.end()))
  {
    throw std::runtime_error("compressedMessageLength out of range: " + compressedMessageLength);
  }
  auto const compressedMessage = std::vector<std::uint8_t>{context.position, context.position += length};
  // TODO Create hash value for compressed message and compare with signature

  auto const uncompressedMessage = Deflator::deflate(compressedMessage);
  return std::move(context);
}
