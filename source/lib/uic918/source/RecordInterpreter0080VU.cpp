
#include "../include/RecordInterpreter0080VU.h"
#include "../include/Utility.h"
#include "../include/SegmentInterpreterEFS.h"

#include "sstream"

RecordInterpreter0080VU::RecordInterpreter0080VU(RecordHeader &&h) : header(std::move(h))
{
  header.ensure("0080VU", {"01"});
}

Context &RecordInterpreter0080VU::interpret(Context &context)
{
  auto const start = context.getPosition() - 12;
  context.addField("0080VU.terminalNummer", std::to_string(Utility::getNumeric16(context.getPosition())));
  context.addField("0080VU.samNummer", std::to_string(Utility::getNumeric24(context.getPosition())));
  context.addField("0080VU.anzahlPersonen", std::to_string(Utility::getNumeric8(context.getPosition())));
  auto const numberOfEfs = Utility::getNumeric8(context.getPosition());
  context.addField("0080VU.anzahlEfs", std::to_string(numberOfEfs));

  for (auto efsIndex = 0; efsIndex < numberOfEfs && !context.isEmpty(); ++efsIndex)
  {
    EFSSegment(std::string("0080VU.efs") + std::to_string(efsIndex) + ".").interpret(context);
  }
  auto const remaining = Utility::getBytes(context.getPosition(), header.recordLength - std::distance(start, context.getPosition()));

  return context;
}