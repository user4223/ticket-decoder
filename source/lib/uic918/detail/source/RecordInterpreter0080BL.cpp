
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
  using json = nlohmann::json;

  static std::map<std::string, std::string> const annotationMap = {
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

  static std::optional<json> toObject(unsigned int size, std::function<std::tuple<std::string, json>()> producer)
  {
    json value = json::object();
    for (auto index = 0; index < size; ++index)
    {
      auto [name, json] = producer();
      value[name] = std::move(json);
    }
    return value.empty() ? std::nullopt : std::make_optional(std::move(value));
  }

  static std::optional<json> toArray(unsigned int size, std::function<json()> producer)
  {
    json value = json::array();
    for (auto index = 0; index < size; ++index)
    {
      value.insert(value.end(), producer());
    }
    return value.empty() ? std::nullopt : std::make_optional(std::move(value));
  }

  static std::map<std::string, std::function<json(Context &)>> const tripInterpreterMap = {
      {std::string("02"), [](auto &context)
       {
         auto const certificate1 = utility::getBytes(context.getPosition(), 11);
         auto const certificate2 = utility::getBytes(context.getPosition(), 11);

         return ::utility::JsonBuilder::object()
             .add("validFrom", utility::getDate8(context.getPosition()))
             .add("validTo", utility::getDate8(context.getPosition()))
             .add("serial", utility::getAlphanumeric(context.getPosition(), 8))
             .value;
       }},
      {std::string("03"), [](auto &context)
       {
         return ::utility::JsonBuilder::object()
             .add("validFrom", utility::getDate8(context.getPosition()))
             .add("validTo", utility::getDate8(context.getPosition()))
             .add("serial", utility::getAlphanumeric(context.getPosition(), 10))
             .value;
       }}};

  RecordInterpreter0080BL::RecordInterpreter0080BL(RecordHeader &&h)
      : AbstractRecord(std::move(h))
  {
    header.ensure("0080BL", {"02", "03"});
  }

  Context &RecordInterpreter0080BL::interpret(Context &context)
  {
    auto const tripInterpreter = tripInterpreterMap.at(header.recordVersion);

    auto recordJson = ::utility::JsonBuilder::object() // clang-format off
      .add("ticketType", utility::getAlphanumeric(context.getPosition(), 2))
      .add("trips", toArray(std::stoi(utility::getAlphanumeric(context.getPosition(), 1)), 
        [&](){ 
          return tripInterpreter(context); }))
      .add("fields", toObject(std::stoi(utility::getAlphanumeric(context.getPosition(), 2)), 
        [&](){
          auto const type = utility::getAlphanumeric(context.getPosition(), 4);
          auto const length = std::stoi(utility::getAlphanumeric(context.getPosition(), 4));
          auto const content = utility::getAlphanumeric(context.getPosition(), length);
          auto const annotation = annotationMap.find(type);
          
          return std::make_tuple(type, ::utility::JsonBuilder::object()
            .add("value", content)
            .add("annotation", annotation == annotationMap.end() 
              ? std::nullopt
              : std::make_optional(annotation->second))
            .value);
        }));

    return context.addRecord(api::Record(header.recordId, header.recordVersion, recordJson.build()));
  }
}
