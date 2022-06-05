
#include "../include/RecordInterpreter0080BL.h"
#include "../include/Utility.h"

#include <stdexcept>

RecordInterpreter0080BL::RecordInterpreter0080BL(RecordHeader &&h) : header(std::move(h)) {}

struct Field
{
  std::string prefix;
  std::string type;
  unsigned int length;
  std::string text;

  Field(Interpreter::BytesType::const_iterator &position)
      : prefix(Utility::getAlphanumeric(position, 1)),
        type(Utility::getAlphanumeric(position, 3)),
        length(std::stoi(Utility::getAlphanumeric(position, 4))),
        text(Utility::getAlphanumeric(position, length))
  {
  }
};

Interpreter::Context &RecordInterpreter0080BL::interpret(Context &context)
{
  if (header.recordId.compare("0080BL") != 0 || header.recordVersion.compare("03") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.to_string());
  }

  context.output.insert(std::make_pair("0080BL.ticketType", Utility::getAlphanumeric(context.position, 2)));

  {
    auto const numberOfTrips = std::stoi(Utility::getAlphanumeric(context.position, 1));
    context.output.insert(std::make_pair("0080BL.numberOfTrips", std::to_string(numberOfTrips)));
    for (auto tripIndex = 0; tripIndex < numberOfTrips && context.position != context.uncompressedMessage.end(); ++tripIndex)
    {
      auto const prefix = std::string("0080BL.trip") + std::to_string(tripIndex) + ".";
      context.output.insert(std::make_pair(prefix + "validFrom", Utility::getAlphanumeric(context.position, 8)));
      context.output.insert(std::make_pair(prefix + "validTo", Utility::getAlphanumeric(context.position, 8)));
      context.output.insert(std::make_pair(prefix + "serial", Utility::getAlphanumeric(context.position, 10)));
    }
  }

  {
    auto const numberOfFields = std::stoi(Utility::getAlphanumeric(context.position, 2));
    context.output.insert(std::make_pair("0080BL.numberOfFields", std::to_string(numberOfFields)));
    for (auto fieldIndex = 0; fieldIndex < numberOfFields && context.position != context.uncompressedMessage.end(); ++fieldIndex)
    {
      auto const field = Field{context.position};
      context.output.insert(std::make_pair(std::string("0080BL.field") + field.type, field.text));
    }
  }

  return context;
}
