
#include "../include/RecordInterpreter0080VU.h"
#include "../include/Utility.h"

#include <stdexcept>

RecordInterpreter0080VU::RecordInterpreter0080VU(RecordHeader &&h) : header(std::move(h)) {}

Context &RecordInterpreter0080VU::interpret(Context &context)
{
  if (header.recordId.compare("0080VU") != 0 || header.recordVersion.compare("01") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.toString());
  }

  context.addField("0080VU.terminalNummer", Utility::getAlphanumeric(context.getPosition(), 2));
  context.addField("0080VU.samNummer", Utility::getAlphanumeric(context.getPosition(), 3));
  context.addField("0080VU.anzahlPersonen", Utility::getAlphanumeric(context.getPosition(), 1));
  // auto const anzahlEfs = std::stoi(Utility::getAlphanumeric(context.getPosition(), 1));
  // context.addField("0080VU.anzahlEfs", std::to_string(anzahlEfs));
  context.addField("0080VU.anzahlEfs", Utility::getAlphanumeric(context.getPosition(), 1));

  auto const remaining = Utility::getBytes(context.getPosition(), header.recordLength - 7 - 12);

  return context;
}
