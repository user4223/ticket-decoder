// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Record0080VU.h"

#include "lib/interpreter/detail/common/include/NumberDecoder.h"
#include "lib/interpreter/detail/common/include/DateTimeDecoder.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/infrastructure/include/Logging.h"

#include "sstream"

namespace interpreter::detail::uic
{

  using namespace common;

  Record0080VU::Record0080VU(infrastructure::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("0080VU", {"01"});
  }

  common::Context Record0080VU::interpret(common::Context &&context)
  {
    auto recordJson = ::utility::JsonBuilder::object(); // clang-format off
    recordJson
      .add("terminalNummer", NumberDecoder::consumeInteger2AsString(context))
      .add("samNummer", NumberDecoder::consumeInteger3AsString(context))
      .add("anzahlPersonen", NumberDecoder::consumeInteger1AsString(context))
      .add("efs", ::utility::toArray(NumberDecoder::consumeInteger1(context), [&context](auto &builder)
        { 
          // TODO Unsure if numeric is the proper interpretation of berechtigungsNummer
          auto const berechtigungsNummer = NumberDecoder::consumeInteger4(context);
          auto const kvpOrganisationsId = NumberDecoder::consumeInteger2(context);
          auto const pvProduktnummer = NumberDecoder::consumeInteger2(context);
          auto const pvOrganisationsId = NumberDecoder::consumeInteger2(context);
          auto const pvProduktbezeichnung = getProduktbezeichnung(pvOrganisationsId, pvProduktnummer);

          builder
            // kvp -> Kundenvertragspartner
            // pv -> Produktverantwortlicher
            .add("berechtigungsNummer", std::to_string(berechtigungsNummer))
            .add("kvpOrganisationsId", std::to_string(kvpOrganisationsId))
            .add("pvProduktnummer", std::to_string(pvProduktnummer))
            .add("pvProduktbezeichnung", pvProduktbezeichnung)
            .add("pvOrganisationsId", std::to_string(pvOrganisationsId))
            .add("gueltigAb", DateTimeDecoder::consumeDateTimeCompact4(context))
            .add("gueltigBis", DateTimeDecoder::consumeDateTimeCompact4(context))
            .add("preis", NumberDecoder::consumeInteger3(context))
            .add("samSequenznummer", NumberDecoder::consumeInteger4AsString(context))
            .add("flaechenelemente", ::utility::toDynamicArray(NumberDecoder::consumeInteger1(context), [&context](auto &builder)
              {
                auto tagStream = std::ostringstream();
                auto const tagValue = int(NumberDecoder::consumeInteger1(context));
                tagStream << std::hex << std::noshowbase << tagValue;
                auto const tag = tagStream.str();
                auto const elementLength = NumberDecoder::consumeInteger1(context);
                auto const typ = NumberDecoder::consumeInteger1AsString(context);
                auto const organisationsId = NumberDecoder::consumeInteger2AsString(context);
                auto const flaechenIdLength = elementLength - 3;
                if (flaechenIdLength != 2 && flaechenIdLength != 3)
                {
                  throw std::runtime_error(std::string("Unexpected FlaechenelementId length: ") + std::to_string(flaechenIdLength)); 
                }
                auto const flaechenId = std::to_string(flaechenIdLength == 2
                  ? NumberDecoder::consumeInteger2(context)
                  : NumberDecoder::consumeInteger3(context));

                builder
                  .add("tag", tag)
                  .add("typ", typ)
                  .add("kvpOrganisationsId", organisationsId)
                  .add("flaechenId", flaechenId);

                return elementLength + 2; })); })); // clang-format on

    context.addRecord(Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }

  /* See 'UIC 918.3-VDV Stammdaten' at https://www.bahn.de/angebot/regio/barcode for updated mappings
   */
  static std::map<std::pair<std::uint16_t, std::uint16_t>, std::string> produktnummerMap = {
      {std::make_pair(6262, 2000), std::string("City-Ticket")},
      {std::make_pair(6263, 1000), std::string("City-mobil Einzelfahrt")},
      {std::make_pair(6263, 1001), std::string("City-mobil Tageskarte")},
      {std::make_pair(6263, 1002), std::string("Baden-Württemberg-Ticket")},
      {std::make_pair(6263, 1004), std::string("Baden-Württemberg-Ticket Nacht")},
      {std::make_pair(6263, 1005), std::string("Bayern-Ticket")},
      {std::make_pair(6263, 1007), std::string("Bayern-Ticket-Nacht")},
      {std::make_pair(6263, 1008), std::string("Brandenburg-Berlin-Ticket")},
      {std::make_pair(6263, 1009), std::string("Brandenburg-Berlin-Ticket-Nacht")},
      {std::make_pair(6263, 1010), std::string("Mecklenburg-Vorpommern-Ticket")},
      {std::make_pair(6263, 1011), std::string("Niedersachsen-Ticket")},
      {std::make_pair(6263, 1012), std::string("Rheinland-Pfalz-Ticket")},
      {std::make_pair(6263, 1013), std::string("Rheinland-Pfalz-Ticket-Nacht")},
      {std::make_pair(6263, 1014), std::string("Saarland-Ticket")},
      {std::make_pair(6263, 1015), std::string("Saarland-Ticket-Nacht")},
      {std::make_pair(6263, 1016), std::string("Sachsen-Anhalt-Ticket")},
      {std::make_pair(6263, 1017), std::string("Sachsen-Ticket")},
      {std::make_pair(6263, 1018), std::string("Schleswig-Holstein-Ticket")},
      {std::make_pair(6263, 1019), std::string("Thüringen-Ticket")},
      {std::make_pair(6263, 1020), std::string("Rheinland-Pfalz-Ticket + Luxemburg")},
      {std::make_pair(6263, 1022), std::string("Bayern-Böhmen-Ticket")},
      {std::make_pair(6263, 1030), std::string("Sachsen-Anhalt-Ticket plus Westharz")},
      {std::make_pair(6263, 1031), std::string("Thüringen-Ticket plus Westharz")},
      {std::make_pair(6263, 1032), std::string("Sachsen-Ticket plus Westharz")},
      {std::make_pair(6263, 1200), std::string("Schönes-Wochenende-Ticket")},
      {std::make_pair(6263, 1201), std::string("Quer-Durchs-Land-Ticket")},
      {std::make_pair(6263, 1202), std::string("9-Euro-Ticket")},
      {std::make_pair(6263, 9999), std::string("Deutschlandticket")},
      {std::make_pair(6263, 3000), std::string("In-Out-System")}};

  std::optional<std::string> Record0080VU::getProduktbezeichnung(std::uint16_t const pvOrgId, std::uint16_t const pvProduktnummer)
  {
    auto const entry = produktnummerMap.find(std::make_pair(pvOrgId, pvProduktnummer));
    return entry == produktnummerMap.end()
               ? std::nullopt
               : std::make_optional(entry->second);
  }
}
