
#include "../include/EFSSegment.h"
#include "../include/Utility.h"

EFSSegment::EFSSegment(Context &context, std::string prefix)
{
  // TODO Unsure if numeric is the proper interpretation of this field
  auto const berechtigungsNummer = Utility::getNumeric32(context.getPosition());
  context.addField(prefix + "berechtigungsNummer", std::to_string(berechtigungsNummer));
  auto const kvpOrganisationsId = Utility::getNumeric16(context.getPosition());
  context.addField(prefix + "kvpOrganisationsId", std::to_string(kvpOrganisationsId));
}