
#include "../include/SegmentInterpreterEFS.h"
#include "../include/Utility.h"

struct EFSFlaechenelement : Interpreter
{
  std::string prefix;

  EFSFlaechenelement(std::string p) : prefix(p) {}

  virtual Context &interpret(Context &context) override
  {
    return context;
  }
};

SegmentInterpreterEFS::SegmentInterpreterEFS(std::string p) : prefix(p) {}

Context &SegmentInterpreterEFS::interpret(Context &context)
{
  // TODO Unsure if numeric is the proper interpretation of this fields
  context.addField(prefix + "berechtigungsNummer", std::to_string(Utility::getNumeric32(context.getPosition())));
  // kvp -> Kundenvertragspartner
  context.addField(prefix + "kvpOrganisationsId", std::to_string(Utility::getNumeric16(context.getPosition())));
  // pv -> Produktverantwortlicher
  context.addField(prefix + "pvProduktnummer", std::to_string(Utility::getNumeric16(context.getPosition())));
  context.addField(prefix + "pvOrganisationsId", std::to_string(Utility::getNumeric16(context.getPosition())));
  context.addField(prefix + "gueltigAb", Utility::getDateTimeCompact(context.getPosition()));
  context.addField(prefix + "gueltigBis", Utility::getDateTimeCompact(context.getPosition()));
  context.addField(prefix + "preis", std::to_string(Utility::getNumeric24(context.getPosition())));
  context.addField(prefix + "samSequenznummer", std::to_string(Utility::getNumeric32(context.getPosition())));
  auto const anzahlFlaechenelemente = Utility::getNumeric8(context.getPosition());
  context.addField(prefix + "anzahlFlaechenelemente", std::to_string(anzahlFlaechenelemente));
  for (auto elementIndex = 0; elementIndex < anzahlFlaechenelemente && !context.isEmpty(); ++elementIndex)
  {
    EFSFlaechenelement{std::string("0080BL.flaechenelement") + std::to_string(elementIndex) + "."}.interpret(context);
  }
  return context;
}