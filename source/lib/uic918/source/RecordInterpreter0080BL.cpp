
#include "../include/RecordInterpreter0080BL.h"
#include "../include/Utility.h"

#include <stdexcept>
#include <sstream>
#include <map>
#include <optional>

RecordInterpreter0080BL::RecordInterpreter0080BL(RecordHeader &&h) : header(std::move(h)) {}

struct BLField
{
  static std::map<std::string, std::string> const typeDescriptionMap;

  std::string type;
  unsigned int length;
  std::string text;
  std::optional<std::string> description;

  BLField(Interpreter::BytesType::const_iterator &position)
      : type(Utility::getAlphanumeric(position, 4)),
        length(std::stoi(Utility::getAlphanumeric(position, 4))),
        text(Utility::getAlphanumeric(position, length)), // clang-format off
        description([this](){ auto const item = typeDescriptionMap.find(type); 
                              return item == typeDescriptionMap.end() 
                                ? std::optional<std::string>() 
                                : std::make_optional(item->second); }()) // clang-format on
  {
  }

  std::string to_string() const
  {
    return description ? (text + " (" + description.value() + ")") : text;
  }
};

std::map<std::string, std::string> const BLField::typeDescriptionMap = {
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
    //{"S041", ""},
    //{"S042", ""},
    //{"S043", ""},
    //{"S044", ""},
    //{"S045", ""},
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
      auto const field = BLField{context.position};
      context.output.insert(std::make_pair(std::string("0080BL.field") + field.type, field.to_string()));
    }
  }

  return context;
}
