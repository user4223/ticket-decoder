
#include "../include/SegmentInterpreterEFS.h"
#include "../include/Utility.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace uic918::detail
{
  struct FlaechenelementListe : Interpreter
  {
    std::string prefix;

    FlaechenelementListe(std::string p) : prefix(p) {}

    virtual Context &interpret(Context &context) override
    {
      context.addField(prefix + "tag", (std::ostringstream() << std::hex << Utility::getNumeric8(context.getPosition())).str());
      auto const length = Utility::getNumeric8(context.getPosition());
      context.addField(prefix + "laenge", std::to_string(length));
      context.addField(prefix + "typ", std::to_string(Utility::getNumeric8(context.getPosition())));
      context.addField(prefix + "kvpOrganisationsId", std::to_string(Utility::getNumeric16(context.getPosition())));
      auto const elementLength = length - 3;
      if (elementLength != 2 && elementLength != 3)
        throw std::runtime_error(std::string("Unexpected Flaechenelement length: ") + std::to_string(elementLength));

      auto const element = elementLength == 2 ? Utility::getNumeric16(context.getPosition()) : Utility::getNumeric24(context.getPosition());
      context.addField(prefix + "flaechenId", std::to_string(element));
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
    auto const tagListenLaenge = Utility::getNumeric8(context.getPosition());
    context.addField(prefix + "flaechenelementListenLaenge", std::to_string(tagListenLaenge));
    FlaechenelementListe{prefix + "flaechenelementListe."}.interpret(context);
    return context;
  }
}