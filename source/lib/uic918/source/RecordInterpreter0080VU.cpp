
#include "../include/RecordInterpreter0080VU.h"
#include "../include/Utility.h"

#include <stdexcept>

RecordInterpreter0080VU::RecordInterpreter0080VU(RecordHeader &&h) : header(std::move(h))
{
  header.ensure("0080VU", {"01"});
}

Context &RecordInterpreter0080VU::interpret(Context &context)
{
  context.addField("0080VU.terminalNummer", Utility::getAlphanumeric(context.getPosition(), 2));
  context.addField("0080VU.samNummer", Utility::getAlphanumeric(context.getPosition(), 3));
  context.addField("0080VU.anzahlPersonen", Utility::getAlphanumeric(context.getPosition(), 1));
  // auto const anzahlEfs = std::stoi(Utility::getAlphanumeric(context.getPosition(), 1));
  // context.addField("0080VU.anzahlEfs", std::to_string(anzahlEfs));
  context.addField("0080VU.anzahlEfs", Utility::getAlphanumeric(context.getPosition(), 1));

  auto const remaining = Utility::getBytes(context.getPosition(), header.recordLength - 7 - 12);

  return context;
}
