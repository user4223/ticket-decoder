
#include "../include/RecordInterpreter0080BL.h"
#include "../include/Utility.h"

#include "../../api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"

#include <sstream>
#include <map>
#include <optional>
#include <functional>
#include <memory>

namespace uic918::detail
{
  static std::map<std::string, std::string> const typeDescriptionMap = {
      //{"S000", ""},
      {"S001", "Tarifbezeichnung"}, // 1 -> Einfache Fahrt, 2 -> Hin- und Rückfahrt
      {"S002", "Produktkategorie"}, // 0 -> RE, 1 -> IC/EC, 2 -> ICE
      {"S003", "Produktklasse Hinfahrt"},
      {"S004", "Produktklasse Rückfahrt"},
      //{"S005", ""},
      //{"S006", ""},
      //{"S007", ""},
      //{"S008", ""},
      {"S009", "Anzahl Personen/Bahncard"},
      //{"S010", ""},
      //{"S011", ""},
      {"S012", "Anzahl Kinder"},
      //{"S013", ""},
      {"S014", "Klasse"},
      {"S015", "Startbahnhof Hinfahrt"},
      {"S016", "Zielbahnhof Hinfahrt"},
      {"S017", "Startbahnhof Rückfahrt"},
      {"S018", "Zielbahnhof Rückfahrt"},
      {"S019", "RIT Vorgangsnummer"},
      {"S020", "RIT Veranstalter"},
      {"S021", "Wegetext"},
      //{"S022", ""},
      {"S023", "Inhaber"},
      {"S024", "Wert"},
      {"S025", "Aufpreis"},
      {"S026", "Preisart"},
      {"S027", "ID-Karte"},
      {"S028", "Vorname#Nachname"},
      //{"S029", ""},
      //{"S030", ""},
      {"S031", "Gültig von"},
      {"S032", "Gültig bis"},
      //{"S033", ""},
      //{"S034", ""},
      {"S035", "EVA-Nummer Startbahnhof"},
      {"S036", "EVA-Nummer Zielbahnhof"},
      //{"S037", ""},
      //{"S038", ""},
      //{"S039", ""},
      {"S040", "Anzahl Personen"},
      {"S041", "Anzahl EFS"},
      //{"S042", ""},
      //{"S043", ""},
      //{"S044", ""},
      //{"S045", ""},
  };

  struct BLField : Interpreter
  {
    std::string prefix;

    BLField(std::string p) : prefix(p) {}

    virtual Context &interpret(Context &context) override
    {
      auto const type = Utility::getAlphanumeric(context.getPosition(), 4);
      auto const length = std::stoi(Utility::getAlphanumeric(context.getPosition(), 4));
      auto const text = Utility::getAlphanumeric(context.getPosition(), length);
      auto const descriptionIterator = typeDescriptionMap.find(type);
      auto const description = descriptionIterator == typeDescriptionMap.end()
                                   ? std::optional<std::string>()
                                   : std::make_optional(descriptionIterator->second);
      context.addField(prefix + type, text, description);
      return context;
    }
  };

  struct BL3Trip : Interpreter
  {
    std::string prefix;

    BL3Trip(std::string p) : prefix(p) {}

    virtual Context &interpret(Context &context) override
    {
      context.addField(prefix + "validFrom", Utility::getDate8(context.getPosition()));
      context.addField(prefix + "validTo", Utility::getDate8(context.getPosition()));
      context.addField(prefix + "serial", Utility::getAlphanumeric(context.getPosition(), 10));
      return context;
    }
  };

  struct BL2Trip : Interpreter
  {
    std::string prefix;

    BL2Trip(std::string p) : prefix() {}

    virtual Context &interpret(Context &context) override
    {
      auto const certificate = Utility::getBytes(context.getPosition(), 11);
      Utility::getBytes(context.getPosition(), 11); // unused
      context.addField(prefix + "validFrom", Utility::getDate8(context.getPosition()));
      context.addField(prefix + "validTo", Utility::getDate8(context.getPosition()));
      context.addField(prefix + "serial", Utility::getAlphanumeric(context.getPosition(), 8));
      return context;
    }
  };

  static std::map<std::string, std::function<std::unique_ptr<Interpreter>(std::string)>> const tripInterpreterMap =
      {
          {std::string("02"), [](auto const &prefix)
           { return std::make_unique<BL2Trip>(prefix); }},
          {std::string("03"), [](auto const &prefix)
           { return std::make_unique<BL3Trip>(prefix); }}};

  RecordInterpreter0080BL::RecordInterpreter0080BL(RecordHeader &&h)
      : AbstractRecord(std::move(h))
  {
    header.ensure("0080BL", {"02", "03"});
  }

  Context &RecordInterpreter0080BL::interpret(Context &context)
  {
    auto json = utility::JsonBuilder::object() // clang-format off
      .add("ticketType", Utility::getAlphanumeric(context.getPosition(), 2));

    auto const tripFactory = tripInterpreterMap.at(header.recordVersion);
    auto const numberOfTrips = std::stoi(Utility::getAlphanumeric(context.getPosition(), 1));
    context.addField("0080BL.numberOfTrips", std::to_string(numberOfTrips));
    for (auto tripIndex = 0; tripIndex < numberOfTrips && !context.isEmpty(); ++tripIndex)
    {
      auto const prefix = std::string("0080BL.trip") + std::to_string(tripIndex) + ".";
      tripFactory(prefix)->interpret(context);
    }

    auto const numberOfFields = std::stoi(Utility::getAlphanumeric(context.getPosition(), 2));
    context.addField("0080BL.numberOfFields", std::to_string(numberOfFields));
    for (auto fieldIndex = 0; fieldIndex < numberOfFields && !context.isEmpty(); ++fieldIndex)
    {
      BLField{"0080BL.field"}.interpret(context);
    }

    return context.addRecord(api::Record(header.recordId, header.recordVersion, json.build()));
  }
}
