
#include "../include/SegmentInterpreterEFS.h"
#include "../include/Utility.h"

EFSSegment::EFSSegment(std::string p) : prefix(p) {}

Context &EFSSegment::interpret(Context &context)
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

  return context;
}