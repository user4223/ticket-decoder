
#include "../include/Record0080BL.h"

#include "lib/interpreter/detail/common/include/Utility.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

#include <sstream>
#include <map>
#include <optional>
#include <functional>
#include <memory>

namespace interpreter::detail::uic
{
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

  static std::map<std::string, std::function<void(Context &, ::utility::JsonBuilder &)>> const tripInterpreterMap =
      {
          {std::string("02"), [](auto &context, auto &builder)
           {
             auto const certificate1 = getBytes(context.getPosition(), 11);
             auto const certificate2 = getBytes(context.getPosition(), 11);

             builder
                 .add("validFrom", getDate8(context.getPosition()))
                 .add("validTo", getDate8(context.getPosition()))
                 .add("serial", getAlphanumeric(context.getPosition(), 8));
           }},
          {std::string("03"), [](auto &context, auto &builder)
           {
             builder
                 .add("validFrom", getDate8(context.getPosition()))
                 .add("validTo", getDate8(context.getPosition()))
                 .add("serial", getAlphanumeric(context.getPosition(), 10));
           }}};

  Record0080BL::Record0080BL(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("0080BL", {"02", "03"});
  }

  Context Record0080BL::interpret(Context &&context)
  {
    auto const tripInterpreter = tripInterpreterMap.at(header.recordVersion);

    auto recordJson = ::utility::JsonBuilder::object(); // clang-format off
    recordJson
      .add("ticketType", getAlphanumeric(context.getPosition(), 2))
      .add("trips", ::utility::toArray(std::stoi(getAlphanumeric(context.getPosition(), 1)), [&](auto &builder)
        { tripInterpreter(context, builder); }))
      .add("fields", ::utility::toObject(std::stoi(getAlphanumeric(context.getPosition(), 2)), [&](auto & builder)
        {
          auto const type = getAlphanumeric(context.getPosition(), 4);
          auto const length = std::stoi(getAlphanumeric(context.getPosition(), 4));
          auto const content = getAlphanumeric(context.getPosition(), length);
          auto const annotation = annotationMap.find(type);

          builder
            .add("value", content)
            .add("annotation", annotation == annotationMap.end() 
              ? std::nullopt
              : std::make_optional(annotation->second));
          return type;
        })); // clang-format on

    context.addRecord(Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }
}
