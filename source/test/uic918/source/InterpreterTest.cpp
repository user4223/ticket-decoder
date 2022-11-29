
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <nlohmann/json.hpp>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/uic918/detail/include/Uic918Interpreter.h"
#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/uic918/api/include/Record.h"

#include "lib/utility/include/Logging.h"

#include "test/support/include/Loader.h"

namespace uic918::detail
{
  using json = nlohmann::json;

  Context interpretFile(std::string fileName)
  {
    auto bytes = ::support::getData(fileName);
    auto loggerFactory = ::utility::LoggerFactory::create();
    auto const signatureChecker = support::getSignatureChecker();
    return detail::Uic918Interpreter(loggerFactory, *signatureChecker).interpret(detail::Context(bytes));
  }

  struct OutputConsumer
  {
    std::map<std::string, Field> output;

    OutputConsumer(std::map<std::string, Field> const &fields) : output(fields) {}

    OutputConsumer(Context &&context) : output(context.getFields()) {}

    std::string consume(std::string key)
    {
      if (!output.contains(key))
      {
        return "key not found";
      }
      auto const value = output.at(key).toString();
      output.erase(key);
      return value;
    }

    std::size_t size() const
    {
      return output.size();
    }

    void dump() const
    {
      std::for_each(output.begin(), output.end(), [](auto const &item)
                    { std::cout << item.first << ": " << item.second.toString() << std::endl; });
    }
  };

  template <size_t S, typename... R>
  std::tuple<R...> get(json node, std::array<std::string, S> keys)
  {
    return std::apply([&](auto... x)
                      { return std::make_tuple((node.find(x) != node.end() ? node[x] : "")...); },
                      keys);
  }

  auto getBLField(json node)
  {
    return get<2, std::string, std::string>(node, {"value", "annotation"});
  }

  auto getTLAYField(json node)
  {
    return get<6, std::string, int, int, int, int, std::string>(node, {"text", "line", "column", "width", "height", "formatting"});
  }

  TEST(UIC918_3_City_Ticket, Metadata)
  {
    auto output = OutputConsumer{interpretFile("Muster 918-3 City-Ticket.raw")};
    EXPECT_EQ(output.size(), 17);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "323");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "455");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL 0080VU");
    EXPECT_EQ(output.consume("validated"), "true");
  }

  TEST(UIC918_3_City_Ticket, Record_U_HEAD)
  {
    auto const context = interpretFile("Muster 918-3 City-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context.getRecord("U_HEAD").getJson());
    EXPECT_EQ(headRecord.size(), 6);
    EXPECT_EQ(headRecord["companyCode"], "0080");
    EXPECT_EQ(headRecord["uniqueTicketKey"], "F4X6XA-3");
    EXPECT_EQ(headRecord["editionTime"], "2020-10-27T13:45:00");
    EXPECT_EQ(headRecord["flags"], "0");
    EXPECT_EQ(headRecord["editionLanguageOfTicket"], "DE");
    EXPECT_EQ(headRecord["secondLanguageOfContract"], "DE");
  }

  TEST(UIC918_3_City_Ticket, Record_0080BL)
  {
    auto const context = interpretFile("Muster 918-3 City-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "315");

    auto const blRecord = json::parse(context.getRecord("0080BL").getJson());
    {
      EXPECT_EQ(blRecord.size(), 3);
      EXPECT_EQ(blRecord["ticketType"], "02");

      auto const fields = blRecord["fields"];
      EXPECT_EQ(fields.size(), 16);
      EXPECT_EQ(getBLField(fields["S001"]), std::make_tuple("Flexpreis", "Tarifbezeichnung"));
      EXPECT_EQ(getBLField(fields["S002"]), std::make_tuple("2", "Produktkategorie"));
      EXPECT_EQ(getBLField(fields["S003"]), std::make_tuple("A", "Produktklasse Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S009"]), std::make_tuple("1-0-0", "Anzahl Personen/Bahncard"));
      EXPECT_EQ(getBLField(fields["S012"]), std::make_tuple("0", "Anzahl Kinder"));
      EXPECT_EQ(getBLField(fields["S014"]), std::make_tuple("S2", "Klasse"));
      EXPECT_EQ(getBLField(fields["S015"]), std::make_tuple("Kassel+City", "Startbahnhof Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S016"]), std::make_tuple("Frankfurt(Main)+City", "Zielbahnhof Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S021"]), std::make_tuple("VIA: GUNT*(BEB*HERS*FD*GELN/COEL*LOLL*GI)", "Wegetext"));
      EXPECT_EQ(getBLField(fields["S023"]), std::make_tuple("Schrift Last", "Inhaber"));
      EXPECT_EQ(getBLField(fields["S026"]), std::make_tuple("12", "Preisart"));
      EXPECT_EQ(getBLField(fields["S028"]), std::make_tuple("Last#Schrift", "Vorname#Nachname"));
      EXPECT_EQ(getBLField(fields["S031"]), std::make_tuple("13.01.2021", "Gültig von"));
      EXPECT_EQ(getBLField(fields["S032"]), std::make_tuple("14.01.2021", "Gültig bis"));
      EXPECT_EQ(getBLField(fields["S035"]), std::make_tuple("3200", "EVA-Nummer Startbahnhof"));
      EXPECT_EQ(getBLField(fields["S036"]), std::make_tuple("105", "EVA-Nummer Zielbahnhof"));
    }
    {
      auto const trips = blRecord["trips"];
      EXPECT_EQ(trips.size(), 1);
      {
        auto const trip = trips[0];
        EXPECT_EQ(trip.size(), 3);
        EXPECT_EQ(trip["validFrom"], "2021-01-13");
        EXPECT_EQ(trip["validTo"], "2021-01-14");
        EXPECT_EQ(trip["serial"], "548746455");
      }
    }
  }

  TEST(UIC918_3_City_Ticket, Record_0080VU)
  {
    auto const context = interpretFile("Muster 918-3 City-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "87");

    auto const vuRecord = json::parse(context.getRecord("0080VU").getJson());
    {
      EXPECT_EQ(vuRecord.size(), 4);
      EXPECT_EQ(vuRecord["terminalNummer"], "100");
      EXPECT_EQ(vuRecord["samNummer"], "0");
      EXPECT_EQ(vuRecord["anzahlPersonen"], "1");

      EXPECT_TRUE(vuRecord.contains("efs"));
      EXPECT_EQ(vuRecord["efs"].size(), 2);
      {
        auto const efs = vuRecord["efs"][0];
        EXPECT_EQ(efs.size(), 9);
        EXPECT_EQ(efs["berechtigungsNummer"], "665659494");
        EXPECT_EQ(efs["gueltigAb"], "2021-01-13T00:00:01");
        EXPECT_EQ(efs["gueltigBis"], "2021-01-14T03:00:00");
        EXPECT_EQ(efs["kvpOrganisationsId"], "6260");
        EXPECT_EQ(efs["preis"], 0);
        EXPECT_EQ(efs["pvOrganisationsId"], "6262");
        EXPECT_EQ(efs["pvProduktnummer"], "2000");
        EXPECT_EQ(efs["samSequenznummer"], "665659494");

        EXPECT_TRUE(efs.contains("flaechenelemente"));
        EXPECT_EQ(efs["flaechenelemente"].size(), 1);
        {
          auto const flaechenelement = efs["flaechenelemente"][0];
          EXPECT_EQ(flaechenelement.size(), 4);
          EXPECT_EQ(flaechenelement["tag"], "dc");
          EXPECT_EQ(flaechenelement["typ"], "13");
          EXPECT_EQ(flaechenelement["flaechenId"], "8003200");
          EXPECT_EQ(flaechenelement["kvpOrganisationsId"], "6262");
        }
      }
      {
        auto const efs = vuRecord["efs"][1];
        EXPECT_EQ(efs.size(), 9);
        EXPECT_EQ(efs["berechtigungsNummer"], "665659495");
        EXPECT_EQ(efs["gueltigAb"], "2021-01-13T00:00:01");
        EXPECT_EQ(efs["gueltigBis"], "2021-01-14T03:00:00");
        EXPECT_EQ(efs["kvpOrganisationsId"], "6260");
        EXPECT_EQ(efs["preis"], 0);
        EXPECT_EQ(efs["pvOrganisationsId"], "6262");
        EXPECT_EQ(efs["pvProduktnummer"], "2000");
        EXPECT_EQ(efs["samSequenznummer"], "665659495");

        EXPECT_TRUE(efs.contains("flaechenelemente"));
        EXPECT_EQ(efs["flaechenelemente"].size(), 1);
        {
          auto const flaechenelement = efs["flaechenelemente"][0];
          EXPECT_EQ(flaechenelement.size(), 4);
          EXPECT_EQ(flaechenelement["tag"], "dc");
          EXPECT_EQ(flaechenelement["typ"], "13");
          EXPECT_EQ(flaechenelement["flaechenId"], "8000105");
          EXPECT_EQ(flaechenelement["kvpOrganisationsId"], "6262");
        }
      }
    }
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Metadata)
  {
    auto output = OutputConsumer{interpretFile("Muster 918-3 Quer-durchs-Land-Ticket.raw")};
    EXPECT_EQ(output.size(), 21);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "346");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "527");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL U_TLAY 0080VU");
    EXPECT_EQ(output.consume("validated"), "true");
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Record_U_HEAD)
  {
    auto const context = interpretFile("Muster 918-3 Quer-durchs-Land-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context.getRecord("U_HEAD").getJson());
    EXPECT_EQ(headRecord.size(), 6);
    EXPECT_EQ(headRecord["companyCode"], "0080");
    EXPECT_EQ(headRecord["uniqueTicketKey"], "EZBG7S-2");
    EXPECT_EQ(headRecord["editionTime"], "2020-10-28T11:49:00");
    EXPECT_EQ(headRecord["flags"], "0");
    EXPECT_EQ(headRecord["editionLanguageOfTicket"], "DE");
    EXPECT_EQ(headRecord["secondLanguageOfContract"], "DE");
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Record_U_TLAY)
  {
    auto const context = interpretFile("Muster 918-3 Quer-durchs-Land-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_TLAY.recordId"), "U_TLAY");
    EXPECT_EQ(output.consume("U_TLAY.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_TLAY.recordLength"), "194");
    EXPECT_EQ(output.consume("U_TLAY.layoutStandard"), "RCT2");

    auto const tlayRecord = json::parse(context.getRecord("U_TLAY").getJson());
    {
      EXPECT_EQ(tlayRecord.size(), 1);
      auto const fields = tlayRecord["fields"];
      EXPECT_EQ(fields.size(), 8);
      EXPECT_EQ(getTLAYField(fields[0]), std::make_tuple("Klasse:", 3, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[5]), std::make_tuple("2", 3, 19, 20, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[1]), std::make_tuple("Pers.:", 4, 0, 20, 2, "0"));
      EXPECT_EQ(getTLAYField(fields[6]), std::make_tuple("1", 4, 19, 20, 2, "1"));
      EXPECT_EQ(getTLAYField(fields[2]), std::make_tuple("Fahrkarte\n", 0, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[7]), std::make_tuple("QUER-DURCHS-LAND-TICKET", 2, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[3]), std::make_tuple("Gültigkeit:", 1, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[4]), std::make_tuple("14.01.2021", 1, 15, 20, 1, "1"));
    }
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Record_0080BL)
  {
    auto const context = interpretFile("Muster 918-3 Quer-durchs-Land-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "228");

    auto const blRecord = json::parse(context.getRecord("0080BL").getJson());
    {
      EXPECT_EQ(blRecord.size(), 3);
      EXPECT_EQ(blRecord["ticketType"], "00");

      auto const fields = blRecord["fields"];
      EXPECT_EQ(fields.size(), 13);
      EXPECT_EQ(getBLField(fields["S001"]), std::make_tuple("Quer-Durchs-Land-Ticket", "Tarifbezeichnung"));
      EXPECT_EQ(getBLField(fields["S002"]), std::make_tuple("0", "Produktkategorie"));
      EXPECT_EQ(getBLField(fields["S003"]), std::make_tuple("C", "Produktklasse Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S009"]), std::make_tuple("1-0-0", "Anzahl Personen/Bahncard"));
      EXPECT_EQ(getBLField(fields["S012"]), std::make_tuple("0", "Anzahl Kinder"));
      EXPECT_EQ(getBLField(fields["S014"]), std::make_tuple("S2", "Klasse"));
      EXPECT_EQ(getBLField(fields["S023"]), std::make_tuple("Schrift Last", "Inhaber"));
      EXPECT_EQ(getBLField(fields["S026"]), std::make_tuple("12", "Preisart"));
      EXPECT_EQ(getBLField(fields["S028"]), std::make_tuple("Last#Schrift", "Vorname#Nachname"));
      EXPECT_EQ(getBLField(fields["S031"]), std::make_tuple("14.01.2021", "Gültig von"));
      EXPECT_EQ(getBLField(fields["S032"]), std::make_tuple("14.01.2021", "Gültig bis"));
      EXPECT_EQ(getBLField(fields["S040"]), std::make_tuple("1", "Anzahl Personen"));
      EXPECT_EQ(getBLField(fields["S041"]), std::make_tuple("1", "Anzahl EFS"));
    }
    {
      auto const trips = blRecord["trips"];
      EXPECT_EQ(trips.size(), 1);
      {
        auto const trip = trips[0];
        EXPECT_EQ(trip.size(), 3);
        EXPECT_EQ(trip["validFrom"], "2021-01-14");
        EXPECT_EQ(trip["validTo"], "2021-01-14");
        EXPECT_EQ(trip["serial"], "548899912");
      }
    }
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Record_0080VU)
  {
    auto const context = interpretFile("Muster 918-3 Quer-durchs-Land-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "52");

    auto const vuRecord = json::parse(context.getRecord("0080VU").getJson());
    {
      EXPECT_EQ(vuRecord.size(), 4);
      EXPECT_EQ(vuRecord["terminalNummer"], "100");
      EXPECT_EQ(vuRecord["samNummer"], "0");
      EXPECT_EQ(vuRecord["anzahlPersonen"], "1");

      EXPECT_TRUE(vuRecord.contains("efs"));
      EXPECT_EQ(vuRecord["efs"].size(), 1);
      {
        auto const efs = vuRecord["efs"][0];
        EXPECT_EQ(efs.size(), 9);
        EXPECT_EQ(efs["berechtigungsNummer"], "665810517");
        EXPECT_EQ(efs["kvpOrganisationsId"], "6260");
        EXPECT_EQ(efs["pvProduktnummer"], "1201");
        EXPECT_EQ(efs["pvOrganisationsId"], "6263");
        EXPECT_EQ(efs["gueltigAb"], "2021-01-14T09:00:00");
        EXPECT_EQ(efs["gueltigBis"], "2021-01-15T03:00:00");
        EXPECT_EQ(efs["preis"], 4200);
        EXPECT_EQ(efs["samSequenznummer"], "665810517");

        EXPECT_TRUE(efs.contains("flaechenelemente"));
        EXPECT_EQ(efs["flaechenelemente"].size(), 1);
        {
          auto const flaechenelement = efs["flaechenelemente"][0];
          EXPECT_EQ(flaechenelement.size(), 4);
          EXPECT_EQ(flaechenelement["tag"], "dc");
          EXPECT_EQ(flaechenelement["typ"], "16");
          EXPECT_EQ(flaechenelement["kvpOrganisationsId"], "5000");
          EXPECT_EQ(flaechenelement["flaechenId"], "1");
        }
      }
    }
  }

  TEST(UIC918_3_City_Mobil_Ticket, Metadata)
  {
    auto output = OutputConsumer{interpretFile("Muster 918-3 City-Mobil Ticket.raw")};
    EXPECT_EQ(output.size(), 17);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "285");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "391");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL 0080VU");
    EXPECT_EQ(output.consume("validated"), "true");
  }

  TEST(UIC918_3_City_Mobil_Ticket, Record_U_HEAD)
  {
    auto const context = interpretFile("Muster 918-3 City-Mobil Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context.getRecord("U_HEAD").getJson());
    EXPECT_EQ(headRecord.size(), 6);
    EXPECT_EQ(headRecord["companyCode"], "0080");
    EXPECT_EQ(headRecord["uniqueTicketKey"], "RPEX4F-4");
    EXPECT_EQ(headRecord["editionTime"], "2020-10-27T13:18:00");
    EXPECT_EQ(headRecord["flags"], "0");
    EXPECT_EQ(headRecord["editionLanguageOfTicket"], "DE");
    EXPECT_EQ(headRecord["secondLanguageOfContract"], "DE");
  }

  TEST(UIC918_3_City_Mobil_Ticket, Record_0080BL)
  {
    auto const context = interpretFile("Muster 918-3 City-Mobil Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "285");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");

    auto const blRecord = json::parse(context.getRecord("0080BL").getJson());
    {
      EXPECT_EQ(blRecord.size(), 3);
      EXPECT_EQ(blRecord["ticketType"], "03");

      auto const fields = blRecord["fields"];
      EXPECT_EQ(fields.size(), 16);
      EXPECT_EQ(getBLField(fields["S001"]), std::make_tuple("Super Sparpreis", "Tarifbezeichnung"));
      EXPECT_EQ(getBLField(fields["S002"]), std::make_tuple("2", "Produktkategorie"));
      EXPECT_EQ(getBLField(fields["S003"]), std::make_tuple("A", "Produktklasse Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S009"]), std::make_tuple("1-1-49", "Anzahl Personen/Bahncard"));
      EXPECT_EQ(getBLField(fields["S012"]), std::make_tuple("0", "Anzahl Kinder"));
      EXPECT_EQ(getBLField(fields["S014"]), std::make_tuple("S2", "Klasse"));
      EXPECT_EQ(getBLField(fields["S015"]), std::make_tuple("Mainz", "Startbahnhof Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S016"]), std::make_tuple("Kassel", "Zielbahnhof Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S021"]), std::make_tuple("NV*F-Flugh 14:09 ICE1196", "Wegetext"));
      EXPECT_EQ(getBLField(fields["S023"]), std::make_tuple("Schrift Last", "Inhaber"));
      EXPECT_EQ(getBLField(fields["S026"]), std::make_tuple("13", "Preisart"));
      EXPECT_EQ(getBLField(fields["S028"]), std::make_tuple("Last#Schrift", "Vorname#Nachname"));
      EXPECT_EQ(getBLField(fields["S031"]), std::make_tuple("11.01.2021", "Gültig von"));
      EXPECT_EQ(getBLField(fields["S032"]), std::make_tuple("11.01.2021", "Gültig bis"));
      EXPECT_EQ(getBLField(fields["S035"]), std::make_tuple("240", "EVA-Nummer Startbahnhof"));
      EXPECT_EQ(getBLField(fields["S036"]), std::make_tuple("3200", "EVA-Nummer Zielbahnhof"));
    }
    {
      auto const trips = blRecord["trips"];
      EXPECT_EQ(trips.size(), 1);
      {
        auto const trip = trips[0];
        EXPECT_EQ(trip.size(), 3);
        EXPECT_EQ(trip["serial"], "548741714");
        EXPECT_EQ(trip["validFrom"], "2021-01-11");
        EXPECT_EQ(trip["validTo"], "2021-01-11");
      }
    }
  }

  TEST(UIC918_3_City_Mobil_Ticket, Record_0080VU)
  {
    auto const context = interpretFile("Muster 918-3 City-Mobil Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "53");

    auto const vuRecord = json::parse(context.getRecord("0080VU").getJson());
    {
      EXPECT_EQ(vuRecord.size(), 4);
      EXPECT_EQ(vuRecord["terminalNummer"], "100");
      EXPECT_EQ(vuRecord["samNummer"], "0");
      EXPECT_EQ(vuRecord["anzahlPersonen"], "1");

      EXPECT_TRUE(vuRecord.contains("efs"));
      EXPECT_EQ(vuRecord["efs"].size(), 1);
      {
        auto const efs = vuRecord["efs"][0];
        EXPECT_EQ(efs.size(), 9);
        EXPECT_EQ(efs["berechtigungsNummer"], "665654772");
        EXPECT_EQ(efs["gueltigAb"], "2021-01-11T00:00:01");
        EXPECT_EQ(efs["gueltigBis"], "2021-01-12T03:00:00");
        EXPECT_EQ(efs["kvpOrganisationsId"], "6260");
        EXPECT_EQ(efs["preis"], 600);
        EXPECT_EQ(efs["pvOrganisationsId"], "6263");
        EXPECT_EQ(efs["pvProduktnummer"], "1001");
        EXPECT_EQ(efs["samSequenznummer"], "665654772");

        EXPECT_TRUE(efs.contains("flaechenelemente"));
        EXPECT_EQ(efs["flaechenelemente"].size(), 1);
        {
          auto const flaechenelement = efs["flaechenelemente"][0];
          EXPECT_EQ(flaechenelement.size(), 4);
          EXPECT_EQ(flaechenelement["tag"], "dc");
          EXPECT_EQ(flaechenelement["typ"], "13");
          EXPECT_EQ(flaechenelement["flaechenId"], "8003200");
          EXPECT_EQ(flaechenelement["kvpOrganisationsId"], "6263");
        }
      }
    }
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Metadata)
  {
    auto output = OutputConsumer{interpretFile("Muster 918-9 Länderticket Sachsen-Anhalt.raw")};
    EXPECT_EQ(output.size(), 21);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "1080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00001");
    EXPECT_EQ(output.consume("compressedMessageLength"), "349");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "404");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD U_TLAY U_FLEX 0080VU");
    EXPECT_EQ(output.consume("validated"), "true");
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Record_U_HEAD)
  {
    auto const context = interpretFile("Muster 918-9 Länderticket Sachsen-Anhalt.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context.getRecord("U_HEAD").getJson());
    EXPECT_EQ(headRecord.size(), 6);
    EXPECT_EQ(headRecord["companyCode"], "1080");
    EXPECT_EQ(headRecord["uniqueTicketKey"], "61B3JR37");
    EXPECT_EQ(headRecord["editionTime"], "2020-11-09T15:17:00");
    EXPECT_EQ(headRecord["flags"], "0");
    EXPECT_EQ(headRecord["editionLanguageOfTicket"], "DE");
    EXPECT_EQ(headRecord["secondLanguageOfContract"], "DE");
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Record_U_TLAY)
  {
    auto const context = interpretFile("Muster 918-9 Länderticket Sachsen-Anhalt.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_TLAY.recordId"), "U_TLAY");
    EXPECT_EQ(output.consume("U_TLAY.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_TLAY.recordLength"), "191");
    EXPECT_EQ(output.consume("U_TLAY.layoutStandard"), "PLAI");

    auto const tlayRecord = json::parse(context.getRecord("U_TLAY").getJson());
    {
      EXPECT_EQ(tlayRecord.size(), 1);
      auto const fields = tlayRecord["fields"];
      EXPECT_EQ(fields.size(), 8);
      EXPECT_EQ(getTLAYField(fields[0]), std::make_tuple("Klasse:", 3, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[5]), std::make_tuple("2", 3, 19, 20, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[1]), std::make_tuple("Pers.:", 4, 0, 20, 2, "0"));
      EXPECT_EQ(getTLAYField(fields[6]), std::make_tuple("1", 4, 19, 20, 2, "1"));
      EXPECT_EQ(getTLAYField(fields[2]), std::make_tuple("Fahrkarte", 0, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[7]), std::make_tuple("Sachsen-Anhalt-Ticket", 2, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[3]), std::make_tuple("Gültigkeit:", 1, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[4]), std::make_tuple("18.11.2020", 1, 15, 20, 1, "1"));
    }
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Record_U_FLEX)
  {
    auto const context = interpretFile("Muster 918-9 Länderticket Sachsen-Anhalt.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_FLEX.recordId"), "U_FLEX");
    EXPECT_EQ(output.consume("U_FLEX.recordVersion"), "13");
    EXPECT_EQ(output.consume("U_FLEX.recordLength"), "108");

    auto const flexRecord = json::parse(context.getRecord("U_FLEX").getJson());
    {
      EXPECT_EQ(flexRecord.size(), 3);
      {
        auto const issuingDetail = flexRecord["issuingDetail"];
        EXPECT_EQ(issuingDetail.size(), 11);
        EXPECT_EQ(issuingDetail["activated"], 1);
        EXPECT_EQ(issuingDetail["currency"], "EUR");
        EXPECT_EQ(issuingDetail["currencyFract"], 2);
        EXPECT_EQ(issuingDetail["issuerName"], "DB AG");
        EXPECT_EQ(issuingDetail["issuerNum"], 1080);
        EXPECT_EQ(issuingDetail["issuerPNR"], "61B3JR37");
        EXPECT_EQ(issuingDetail["issuingDate"], "2020-11-09");
        EXPECT_EQ(issuingDetail["issuingTime"], 917);
        EXPECT_EQ(issuingDetail["securePaperTicket"], 0);
        EXPECT_EQ(issuingDetail["securityProviderNum"], 1080);
        EXPECT_EQ(issuingDetail["specimen"], 0);
      }
      {
        auto const travelerDetail = flexRecord["travelerDetail"];
        EXPECT_EQ(travelerDetail.size(), 1);

        EXPECT_EQ(travelerDetail["traveler"].size(), 1);
        {
          auto const travelers0 = travelerDetail["traveler"][0];
          EXPECT_EQ(travelers0.size(), 3);
          EXPECT_EQ(travelers0["firstName"], "Karsten");
          EXPECT_EQ(travelers0["lastName"], "Will");
          EXPECT_EQ(travelers0["ticketHolder"], 1);
        }
      }
      {
        auto const transportDocuments = flexRecord["transportDocuments"];
        EXPECT_EQ(transportDocuments.size(), 1);
        EXPECT_EQ(transportDocuments[0].size(), 1);
        {
          auto const openTicket0 = transportDocuments[0]["openTicket"];
          EXPECT_EQ(openTicket0.size(), 11);
          EXPECT_EQ(openTicket0["classCode"], "2");
          EXPECT_EQ(openTicket0["productId"], "Fahrkarte");
          EXPECT_EQ(openTicket0["reference"], "FTJ9KNEM");
          EXPECT_EQ(openTicket0["returnIncluded"], 0);
          EXPECT_EQ(openTicket0["stationCodeTable"], "0");
          EXPECT_EQ(openTicket0["validFromDay"], 9);
          EXPECT_EQ(openTicket0["validFromTime"], 0);
          EXPECT_EQ(openTicket0["validFromUTCOffset"], -4);
          EXPECT_EQ(openTicket0["validUntilDay"], 1);
          EXPECT_EQ(openTicket0["validUntilTime"], 180);
          EXPECT_EQ(openTicket0["tariffs"].size(), 1);
          {
            auto const tariffs0 = openTicket0["tariffs"][0];
            EXPECT_EQ(tariffs0.size(), 4);
            EXPECT_EQ(tariffs0["numberOfPassengers"], 1);
            EXPECT_EQ(tariffs0["passengerType"], "0");
            EXPECT_EQ(tariffs0["restrictedToCountryOfResidence"], 0);
            EXPECT_EQ(tariffs0["tariffDesc"], "Sachsen-Anhalt-Ticket");
          }
        }
      }
    }
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Record_0080VU)
  {
    auto const context = interpretFile("Muster 918-9 Länderticket Sachsen-Anhalt.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "52");

    auto const vuRecord = json::parse(context.getRecord("0080VU").getJson());
    {
      EXPECT_EQ(vuRecord.size(), 4);
      EXPECT_EQ(vuRecord["terminalNummer"], "100");
      EXPECT_EQ(vuRecord["samNummer"], "0");
      EXPECT_EQ(vuRecord["anzahlPersonen"], "1");

      EXPECT_TRUE(vuRecord.contains("efs"));
      EXPECT_EQ(vuRecord["efs"].size(), 1);
      {
        auto const efs = vuRecord["efs"][0];
        EXPECT_EQ(efs.size(), 9);
        EXPECT_EQ(efs["berechtigungsNummer"], "785689900");
        EXPECT_EQ(efs["gueltigAb"], "2020-11-18T00:00:00");
        EXPECT_EQ(efs["gueltigBis"], "2020-11-19T03:00:00");
        EXPECT_EQ(efs["kvpOrganisationsId"], "6260");
        EXPECT_EQ(efs["preis"], 2400);
        EXPECT_EQ(efs["pvOrganisationsId"], "6263");
        EXPECT_EQ(efs["pvProduktnummer"], "1016");
        EXPECT_EQ(efs["samSequenznummer"], "785689900");

        EXPECT_TRUE(efs.contains("flaechenelemente"));
        EXPECT_EQ(efs["flaechenelemente"].size(), 1);
        {
          auto const flaechenelement = efs["flaechenelemente"][0];
          EXPECT_EQ(flaechenelement.size(), 4);
          EXPECT_EQ(flaechenelement["tag"], "dc");
          EXPECT_EQ(flaechenelement["typ"], "16");
          EXPECT_EQ(flaechenelement["flaechenId"], "15");
          EXPECT_EQ(flaechenelement["kvpOrganisationsId"], "5000");
        }
      }
    }
  }

  TEST(UIC918_9_FV_SuperSparpreis, Metadata)
  {
    auto output = OutputConsumer{interpretFile("Muster 918-9 FV_SuperSparpreis.raw")};
    EXPECT_EQ(output.size(), 11);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "02");
    EXPECT_EQ(output.consume("companyCode"), "1080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00002");
    EXPECT_EQ(output.consume("compressedMessageLength"), "199");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "188");
    EXPECT_EQ(output.consume("recordIds"), "U_FLEX");
    EXPECT_EQ(output.consume("validated"), "true");
  }

  TEST(UIC918_9_FV_SuperSparpreis, Record_U_FLEX)
  {
    auto const context = interpretFile("Muster 918-9 FV_SuperSparpreis.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_FLEX.recordId"), "U_FLEX");
    EXPECT_EQ(output.consume("U_FLEX.recordVersion"), "13");
    EXPECT_EQ(output.consume("U_FLEX.recordLength"), "188");

    auto const flexRecord = json::parse(context.getRecord("U_FLEX").getJson());
    {
      EXPECT_EQ(flexRecord.size(), 3);
      {
        auto const issuingDetail = flexRecord["issuingDetail"];
        EXPECT_EQ(issuingDetail.size(), 11);
        EXPECT_EQ(issuingDetail["activated"], 1);
        EXPECT_EQ(issuingDetail["currency"], "EUR");
        EXPECT_EQ(issuingDetail["currencyFract"], 2);
        EXPECT_EQ(issuingDetail["issuerName"], "DB AG");
        EXPECT_EQ(issuingDetail["issuerNum"], 1080);
        EXPECT_EQ(issuingDetail["issuerPNR"], "D260V48G");
        EXPECT_EQ(issuingDetail["issuingDate"], "2020-10-30");
        EXPECT_EQ(issuingDetail["issuingTime"], 650);
        EXPECT_EQ(issuingDetail["securePaperTicket"], 0);
        EXPECT_EQ(issuingDetail["securityProviderNum"], 1080);
        EXPECT_EQ(issuingDetail["specimen"], 0);
      }
      {
        auto const travelerDetail = flexRecord["travelerDetail"];
        EXPECT_EQ(travelerDetail.size(), 1);
        {
          EXPECT_EQ(travelerDetail["traveler"].size(), 1);
          auto const travelers0 = travelerDetail["traveler"][0];
          EXPECT_EQ(travelers0.size(), 3);
          EXPECT_EQ(travelers0["firstName"], "Karsten");
          EXPECT_EQ(travelers0["lastName"], "Will");
          EXPECT_EQ(travelers0["ticketHolder"], 1);
        }
      }
      {
        auto const transportDocuments = flexRecord["transportDocuments"];
        EXPECT_EQ(transportDocuments.size(), 1);
        EXPECT_EQ(transportDocuments[0].size(), 1);
        {
          auto const openTicket0 = transportDocuments[0]["openTicket"];
          EXPECT_EQ(openTicket0.size(), 16);
          EXPECT_EQ(openTicket0["classCode"], "2");
          EXPECT_EQ(openTicket0["fromStationName"], "Mannheim");
          EXPECT_EQ(openTicket0["fromStationNum"], 8000244);
          EXPECT_EQ(openTicket0["productId"], "ICE Fahrkarte");
          EXPECT_EQ(openTicket0["reference"], "CN0CTUMY");
          EXPECT_EQ(openTicket0["returnIncluded"], 0);
          EXPECT_EQ(openTicket0["stationCodeTable"], "4");
          EXPECT_EQ(openTicket0["toStationName"], "Reutlingen");
          EXPECT_EQ(openTicket0["toStationNum"], 8000314);
          EXPECT_EQ(openTicket0["validFromDay"], 6);
          EXPECT_EQ(openTicket0["validFromTime"], 0);
          EXPECT_EQ(openTicket0["validFromUTCOffset"], -4);
          EXPECT_EQ(openTicket0["validRegionDesc"], "Via: <1080>(HD*BR*BRT/GRAB*KA*PF)*VAI*S*PLO");
          EXPECT_EQ(openTicket0["validUntilDay"], 1);
          EXPECT_EQ(openTicket0["validUntilTime"], 600);
          EXPECT_EQ(openTicket0["tariffs"].size(), 1);
          {
            auto const tariffs0 = openTicket0["tariffs"][0];
            EXPECT_EQ(tariffs0.size(), 4);
            EXPECT_EQ(tariffs0["numberOfPassengers"], 1);
            EXPECT_EQ(tariffs0["passengerType"], "0");
            EXPECT_EQ(tariffs0["restrictedToCountryOfResidence"], 0);
            EXPECT_EQ(tariffs0["tariffDesc"], "Super Sparpreis");
          }
        }
      }
    }
  }

  TEST(UIC918_3_Schleswig_Holstein_Ticket, Metadata)
  {
    auto output = OutputConsumer{interpretFile("Muster 918-3 Schleswig-Holstein-Ticket.raw")};
    EXPECT_EQ(output.size(), 21);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "351");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "531");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL U_TLAY 0080VU");
    EXPECT_EQ(output.consume("validated"), "true");
  }

  TEST(UIC918_3_Schleswig_Holstein_Ticket, Record_U_HEAD)
  {
    auto const context = interpretFile("Muster 918-3 Schleswig-Holstein-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context.getRecord("U_HEAD").getJson());
    EXPECT_EQ(headRecord.size(), 6);
    EXPECT_EQ(headRecord["companyCode"], "0080");
    EXPECT_EQ(headRecord["uniqueTicketKey"], "DPHH1D-2");
    EXPECT_EQ(headRecord["editionTime"], "2020-10-28T11:46:00");
    EXPECT_EQ(headRecord["flags"], "0");
    EXPECT_EQ(headRecord["editionLanguageOfTicket"], "DE");
    EXPECT_EQ(headRecord["secondLanguageOfContract"], "DE");
  }

  TEST(UIC918_3_Schleswig_Holstein_Ticket, Record_0080BL)
  {
    auto const context = interpretFile("Muster 918-3 Schleswig-Holstein-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "230");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");

    auto const blRecord = json::parse(context.getRecord("0080BL").getJson());
    {
      EXPECT_EQ(blRecord.size(), 3);
      EXPECT_EQ(blRecord["ticketType"], "00");

      auto const fields = blRecord["fields"];
      EXPECT_EQ(fields.size(), 13);
      EXPECT_EQ(getBLField(fields["S001"]), std::make_tuple("Schleswig-Holstein-Ticket", "Tarifbezeichnung"));
      EXPECT_EQ(getBLField(fields["S002"]), std::make_tuple("0", "Produktkategorie"));
      EXPECT_EQ(getBLField(fields["S003"]), std::make_tuple("C", "Produktklasse Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S009"]), std::make_tuple("2-0-0", "Anzahl Personen/Bahncard"));
      EXPECT_EQ(getBLField(fields["S012"]), std::make_tuple("0", "Anzahl Kinder"));
      EXPECT_EQ(getBLField(fields["S014"]), std::make_tuple("S2", "Klasse"));
      EXPECT_EQ(getBLField(fields["S023"]), std::make_tuple("Schrift Last", "Inhaber"));
      EXPECT_EQ(getBLField(fields["S026"]), std::make_tuple("12", "Preisart"));
      EXPECT_EQ(getBLField(fields["S028"]), std::make_tuple("Last#Schrift", "Vorname#Nachname"));
      EXPECT_EQ(getBLField(fields["S031"]), std::make_tuple("13.01.2021", "Gültig von"));
      EXPECT_EQ(getBLField(fields["S032"]), std::make_tuple("13.01.2021", "Gültig bis"));
      EXPECT_EQ(getBLField(fields["S040"]), std::make_tuple("2", "Anzahl Personen"));
      EXPECT_EQ(getBLField(fields["S041"]), std::make_tuple("1", "Anzahl EFS"));
    }
    {
      auto const trips = blRecord["trips"];
      EXPECT_EQ(trips.size(), 1);
      {
        auto const trip = trips[0];
        EXPECT_EQ(trip.size(), 3);
        EXPECT_EQ(trip["serial"], "548899493");
        EXPECT_EQ(trip["validFrom"], "2021-01-13");
        EXPECT_EQ(trip["validTo"], "2021-01-13");
      }
    }
  }

  TEST(UIC918_3_Schleswig_Holstein_Ticket, Record_U_TLAY)
  {
    auto const context = interpretFile("Muster 918-3 Schleswig-Holstein-Ticket.raw");
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_TLAY.recordId"), "U_TLAY");
    EXPECT_EQ(output.consume("U_TLAY.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_TLAY.recordLength"), "196");
    EXPECT_EQ(output.consume("U_TLAY.layoutStandard"), "RCT2");

    auto const tlayRecord = json::parse(context.getRecord("U_TLAY").getJson());
    {
      EXPECT_EQ(tlayRecord.size(), 1);
      auto const fields = tlayRecord["fields"];
      EXPECT_EQ(fields.size(), 8);
      EXPECT_EQ(getTLAYField(fields[0]), std::make_tuple("Klasse:", 3, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[5]), std::make_tuple("2", 3, 19, 20, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[1]), std::make_tuple("Pers.:", 4, 0, 20, 2, "0"));
      EXPECT_EQ(getTLAYField(fields[6]), std::make_tuple("2", 4, 19, 20, 2, "1"));
      EXPECT_EQ(getTLAYField(fields[2]), std::make_tuple("Fahrkarte\n", 0, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[7]), std::make_tuple("SCHLESWIG-HOLSTEIN-TICKET", 2, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[3]), std::make_tuple("Gültigkeit:", 1, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[4]), std::make_tuple("13.01.2021", 1, 15, 20, 1, "1"));
    }
  }

  TEST(EUR9_Ticket, Metadata)
  {
    auto const context = interpretFile("9EUR_Ticket.raw");
    if (!context.hasInput())
    {
      GTEST_SKIP() << "Input data/file not found";
    }
    auto output = OutputConsumer{context.getFields()};
    EXPECT_EQ(output.size(), 21);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "336");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "507");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL U_TLAY 0080VU");
    EXPECT_EQ(output.consume("validated"), "true");
  }

  TEST(EUR9_Ticket, Record_U_HEAD)
  {
    auto const context = interpretFile("9EUR_Ticket.raw");
    if (!context.hasInput())
    {
      GTEST_SKIP() << "Input data/file not found";
    }
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context.getRecord("U_HEAD").getJson());
    EXPECT_EQ(headRecord.size(), 6);
    EXPECT_EQ(headRecord["companyCode"], "0080");
    EXPECT_EQ(headRecord["uniqueTicketKey"], "LXVW31-2");
    EXPECT_EQ(headRecord["editionTime"], "2022-07-06T23:13:00");
    EXPECT_EQ(headRecord["flags"], "0");
    EXPECT_EQ(headRecord["editionLanguageOfTicket"], "DE");
    EXPECT_EQ(headRecord["secondLanguageOfContract"], "DE");
  }

  TEST(EUR9_Ticket, Record_0080BL)
  {
    auto const context = interpretFile("9EUR_Ticket.raw");
    if (!context.hasInput())
    {
      GTEST_SKIP() << "Input data/file not found";
    }
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "218");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");

    auto const blRecord = json::parse(context.getRecord("0080BL").getJson());
    {
      EXPECT_EQ(blRecord.size(), 3);
      EXPECT_EQ(blRecord["ticketType"], "00");

      auto const fields = blRecord["fields"];
      EXPECT_EQ(fields.size(), 13);
      EXPECT_EQ(getBLField(fields["S001"]), std::make_tuple("9-Euro-Ticket", "Tarifbezeichnung"));
      EXPECT_EQ(getBLField(fields["S002"]), std::make_tuple("0", "Produktkategorie"));
      EXPECT_EQ(getBLField(fields["S003"]), std::make_tuple("C", "Produktklasse Hinfahrt"));
      EXPECT_EQ(getBLField(fields["S009"]), std::make_tuple("1-0-0", "Anzahl Personen/Bahncard"));
      EXPECT_EQ(getBLField(fields["S012"]), std::make_tuple("0", "Anzahl Kinder"));
      EXPECT_EQ(getBLField(fields["S014"]), std::make_tuple("S2", "Klasse"));
      // EXPECT_EQ(getBLField(fields["S023"]), std::make_tuple("Schrift Last", "Inhaber"));
      EXPECT_EQ(getBLField(fields["S026"]), std::make_tuple("12", "Preisart"));
      // EXPECT_EQ(getBLField(fields["S028"]), std::make_tuple("Last#Schrift", "Vorname#Nachname"));
      EXPECT_EQ(getBLField(fields["S031"]), std::make_tuple("01.07.2022", "Gültig von"));
      EXPECT_EQ(getBLField(fields["S032"]), std::make_tuple("31.07.2022", "Gültig bis"));
      EXPECT_EQ(getBLField(fields["S040"]), std::make_tuple("1", "Anzahl Personen"));
      EXPECT_EQ(getBLField(fields["S041"]), std::make_tuple("1", "Anzahl EFS"));
    }
    {
      auto const trips = blRecord["trips"];
      EXPECT_EQ(trips.size(), 1);
      {
        auto const trip = trips[0];
        EXPECT_EQ(trip.size(), 3);
        EXPECT_EQ(trip["serial"], "716850055");
        EXPECT_EQ(trip["validFrom"], "2022-07-01");
        EXPECT_EQ(trip["validTo"], "2022-07-31");
      }
    }
  }

  TEST(EUR9_Ticket, Record_U_TLAY)
  {
    auto const context = interpretFile("9EUR_Ticket.raw");
    if (!context.hasInput())
    {
      GTEST_SKIP() << "Input data/file not found";
    }
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("U_TLAY.recordId"), "U_TLAY");
    EXPECT_EQ(output.consume("U_TLAY.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_TLAY.recordLength"), "184");
    EXPECT_EQ(output.consume("U_TLAY.layoutStandard"), "RCT2");

    auto const tlayRecord = json::parse(context.getRecord("U_TLAY").getJson());
    {
      EXPECT_EQ(tlayRecord.size(), 1);
      auto const fields = tlayRecord["fields"];
      EXPECT_EQ(fields.size(), 8);
      EXPECT_EQ(getTLAYField(fields[0]), std::make_tuple("Klasse:", 3, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[5]), std::make_tuple("2", 3, 19, 20, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[1]), std::make_tuple("Pers.:", 4, 0, 20, 2, "0"));
      EXPECT_EQ(getTLAYField(fields[6]), std::make_tuple("1", 4, 19, 20, 2, "1"));
      EXPECT_EQ(getTLAYField(fields[2]), std::make_tuple("Fahrkarte\n", 0, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[7]), std::make_tuple("9-EURO-TICKET", 2, 0, 80, 1, "1"));
      EXPECT_EQ(getTLAYField(fields[3]), std::make_tuple("Gültigkeit:", 1, 0, 20, 1, "0"));
      EXPECT_EQ(getTLAYField(fields[4]), std::make_tuple("01.07.2022", 1, 15, 20, 1, "1"));
    }
  }

  TEST(EUR9_Ticket, Record_0080VU)
  {
    auto const context = interpretFile("9EUR_Ticket.raw");
    if (!context.hasInput())
    {
      GTEST_SKIP() << "Input data/file not found";
    }
    auto output = OutputConsumer{context.getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "52");

    auto const vuRecord = json::parse(context.getRecord("0080VU").getJson());
    {
      EXPECT_EQ(vuRecord.size(), 4);
      EXPECT_EQ(vuRecord["terminalNummer"], "100");
      EXPECT_EQ(vuRecord["samNummer"], "0");
      EXPECT_EQ(vuRecord["anzahlPersonen"], "1");

      EXPECT_TRUE(vuRecord.contains("efs"));
      EXPECT_EQ(vuRecord["efs"].size(), 1);
      {
        auto const efs = vuRecord["efs"][0];
        EXPECT_EQ(efs.size(), 9);
        EXPECT_EQ(efs["berechtigungsNummer"], "866503297");
        EXPECT_EQ(efs["gueltigAb"], "2022-07-01T00:00:00");
        EXPECT_EQ(efs["gueltigBis"], "2022-07-31T23:59:00");
        EXPECT_EQ(efs["kvpOrganisationsId"], "6260");
        EXPECT_EQ(efs["preis"], 900);
        EXPECT_EQ(efs["pvOrganisationsId"], "6263");
        EXPECT_EQ(efs["pvProduktnummer"], "1202");
        EXPECT_EQ(efs["samSequenznummer"], "866503297");

        EXPECT_TRUE(efs.contains("flaechenelemente"));
        EXPECT_EQ(efs["flaechenelemente"].size(), 1);
        {
          auto const flaechenelement = efs["flaechenelemente"][0];
          EXPECT_EQ(flaechenelement.size(), 4);
          EXPECT_EQ(flaechenelement["tag"], "dc");
          EXPECT_EQ(flaechenelement["typ"], "16");
          EXPECT_EQ(flaechenelement["flaechenId"], "1");
          EXPECT_EQ(flaechenelement["kvpOrganisationsId"], "5000");
        }
      }
    }
  }

  TEST(BVG_4Fahrtenkarte, Metadata)
  {
    auto const context = interpretFile("BVG-4Fahrtenkarte.raw");
    if (!context.hasInput())
    {
      GTEST_SKIP() << "Input data/file not found";
    }
    auto output = OutputConsumer{context.getFields()};
    EXPECT_EQ(output.size(), 0); // just not uic918
  }

  TEST(Unknown_Ticket1, Metadata)
  {
    auto const context = interpretFile("Unknown Ticket1.raw");
    if (!context.hasInput())
    {
      GTEST_SKIP() << "Input data/file not found";
    }
    auto output = OutputConsumer{context.getFields()};
    EXPECT_EQ(output.size(), 14);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "02");
    EXPECT_EQ(output.consume("companyCode"), "1080");
    // EXPECT_EQ(output.consume("signatureKeyId"), "00001");
    EXPECT_EQ(output.consume("compressedMessageLength"), "253");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "272");
    EXPECT_EQ(output.consume("recordIds"), "U_FLEX 0080VU");
    EXPECT_EQ(output.consume("validated"), "false");
    // output.dump();
    // std::cout << json::parse(context->getRecord("U_FLEX").getJson()).dump(3) << std::endl;
  }
}
