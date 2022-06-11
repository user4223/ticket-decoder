
#include "../include/RecordInterpreter0080VU.h"
#include "../include/Utility.h"

#include <stdexcept>

RecordInterpreter0080VU::RecordInterpreter0080VU(RecordHeader &&h) : header(std::move(h)) {}

Interpreter::Context &RecordInterpreter0080VU::interpret(Context &context)
{
  if (header.recordId.compare("0080VU") != 0 || header.recordVersion.compare("01") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.toString());
  }

  context.output.insert(Interpreter::Field::createEntry("0080VU.terminalNummer", Utility::getAlphanumeric(context.position, 2)));
  context.output.insert(Interpreter::Field::createEntry("0080VU.samNummer", Utility::getAlphanumeric(context.position, 3)));
  context.output.insert(Interpreter::Field::createEntry("0080VU.anzahlPersonen", Utility::getAlphanumeric(context.position, 1)));
  // auto const anzahlEfs = std::stoi(Utility::getAlphanumeric(context.position, 1));
  context.output.insert(Interpreter::Field::createEntry("0080VU.anzahlEfs", Utility::getAlphanumeric(context.position, 1)));

  auto all = Interpreter::BytesType{context.position, context.position + header.payloadLength};

  return context;
}
