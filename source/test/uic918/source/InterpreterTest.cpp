
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <nlohmann/json.hpp>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/uic918/detail/include/Interpreter.h"
#include "lib/uic918/api/include/Record.h"

namespace uic918::detail
{
  using json = nlohmann::json;

  Context::BytesType getData(std::string fileName)
  {
    auto const path = std::filesystem::path("..").append("uic918").append("etc").append(fileName);
    EXPECT_TRUE(std::filesystem::exists(path));
    auto ifs = std::ifstream(path, std::ios::binary | std::ios::ate);
    auto const size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    EXPECT_GT(size, 0);

    auto buffer = Context::BytesType(size);
    EXPECT_TRUE(ifs.read(reinterpret_cast<char *>(buffer.data()), size));
    return buffer;
  }

  struct OutputConsumer
  {
    std::map<std::string, Field> output;

    OutputConsumer(std::map<std::string, Field> const &fields) : output(fields) {}

    OutputConsumer(std::unique_ptr<Context> &&context) : output(context->getFields()) {}

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

  std::tuple<std::string, std::string> get(json node, std::tuple<std::string, std::string> keys)
  {
    if (!(node.contains(std::get<0>(keys)) && node.contains(std::get<1>(keys))))
    {
      return {};
    }
    return {node[std::get<0>(keys)], node[std::get<1>(keys)]};
  }

  std::tuple<std::string, std::string> getBLField(json node)
  {
    return get(node, {"value", "annotation"});
  }

  TEST(UIC918_3_City_Ticket, Metadata)
  {
    auto output = OutputConsumer{Interpreter::interpret(getData("Muster 918-3 City-Ticket.raw"))};
    EXPECT_EQ(output.size(), 48);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "323");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "455");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL 0080VU");
  }

  TEST(UIC918_3_City_Ticket, Record_U_HEAD)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-3 City-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context->getRecord("U_HEAD").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-3 City-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "315");

    auto const blRecord = json::parse(context->getRecord("0080BL").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-3 City-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "87");

    EXPECT_EQ(output.consume("0080VU.terminalNummer"), "100");
    EXPECT_EQ(output.consume("0080VU.samNummer"), "0");
    EXPECT_EQ(output.consume("0080VU.anzahlPersonen"), "1");

    EXPECT_EQ(output.consume("0080VU.anzahlEfs"), "2");
    {
      EXPECT_EQ(output.consume("0080VU.efs0.berechtigungsNummer"), "665659494");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigAb"), "2021-01-13T00:00:01");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigBis"), "2021-01-14T03:00:00");
      EXPECT_EQ(output.consume("0080VU.efs0.kvpOrganisationsId"), "6260");
      EXPECT_EQ(output.consume("0080VU.efs0.preis"), "0");
      EXPECT_EQ(output.consume("0080VU.efs0.pvOrganisationsId"), "6262");
      EXPECT_EQ(output.consume("0080VU.efs0.pvProduktnummer"), "2000");
      EXPECT_EQ(output.consume("0080VU.efs0.samSequenznummer"), "665659494");

      EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListenLaenge"), "8");
      {
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.tag"), "\xDC");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.laenge"), "6");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.typ"), "13");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.flaechenId"), "8003200");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.kvpOrganisationsId"), "6262");
      }
    }
    {
      EXPECT_EQ(output.consume("0080VU.efs1.berechtigungsNummer"), "665659495");
      EXPECT_EQ(output.consume("0080VU.efs1.gueltigAb"), "2021-01-13T00:00:01");
      EXPECT_EQ(output.consume("0080VU.efs1.gueltigBis"), "2021-01-14T03:00:00");
      EXPECT_EQ(output.consume("0080VU.efs1.kvpOrganisationsId"), "6260");
      EXPECT_EQ(output.consume("0080VU.efs1.preis"), "0");
      EXPECT_EQ(output.consume("0080VU.efs1.pvOrganisationsId"), "6262");
      EXPECT_EQ(output.consume("0080VU.efs1.pvProduktnummer"), "2000");
      EXPECT_EQ(output.consume("0080VU.efs1.samSequenznummer"), "665659495");

      EXPECT_EQ(output.consume("0080VU.efs1.flaechenelementListenLaenge"), "8");
      {
        EXPECT_EQ(output.consume("0080VU.efs1.flaechenelementListe.tag"), "\xDC");
        EXPECT_EQ(output.consume("0080VU.efs1.flaechenelementListe.laenge"), "6");
        EXPECT_EQ(output.consume("0080VU.efs1.flaechenelementListe.typ"), "13");
        EXPECT_EQ(output.consume("0080VU.efs1.flaechenelementListe.flaechenId"), "8000105");
        EXPECT_EQ(output.consume("0080VU.efs1.flaechenelementListe.kvpOrganisationsId"), "6262");
      }
    }
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Metadata)
  {
    auto output = OutputConsumer{Interpreter::interpret(getData("Muster 918-3 Quer-durchs-Land-Ticket.raw"))};
    EXPECT_EQ(output.size(), 55);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "346");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "527");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL U_TLAY 0080VU");
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Record_U_HEAD)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-3 Quer-durchs-Land-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context->getRecord("U_HEAD").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-3 Quer-durchs-Land-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_TLAY.recordId"), "U_TLAY");
    EXPECT_EQ(output.consume("U_TLAY.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_TLAY.recordLength"), "194");

    EXPECT_EQ(output.consume("U_TLAY.layoutStandard"), "RCT2");
    EXPECT_EQ(output.consume("U_TLAY.numberOfFields"), "8");
    {
      EXPECT_EQ(output.consume("U_TLAY.field0000.text"), "Klasse:");
      EXPECT_EQ(output.consume("U_TLAY.field0000.format"), "L3, C0, W20, H1, F0");
      EXPECT_EQ(output.consume("U_TLAY.field0005.text"), "2");
      EXPECT_EQ(output.consume("U_TLAY.field0005.format"), "L3, C19, W20, H1, F1");

      EXPECT_EQ(output.consume("U_TLAY.field0001.text"), "Pers.:");
      EXPECT_EQ(output.consume("U_TLAY.field0001.format"), "L4, C0, W20, H2, F0");
      EXPECT_EQ(output.consume("U_TLAY.field0006.text"), "1");
      EXPECT_EQ(output.consume("U_TLAY.field0006.format"), "L4, C19, W20, H2, F1");

      EXPECT_EQ(output.consume("U_TLAY.field0002.text"), "Fahrkarte\n");
      EXPECT_EQ(output.consume("U_TLAY.field0002.format"), "L0, C0, W80, H1, F1");
      EXPECT_EQ(output.consume("U_TLAY.field0007.text"), "QUER-DURCHS-LAND-TICKET");
      EXPECT_EQ(output.consume("U_TLAY.field0007.format"), "L2, C0, W80, H1, F1");

      EXPECT_EQ(output.consume("U_TLAY.field0003.text"), "Gültigkeit:");
      EXPECT_EQ(output.consume("U_TLAY.field0003.format"), "L1, C0, W20, H1, F0");
      EXPECT_EQ(output.consume("U_TLAY.field0004.text"), "14.01.2021");
      EXPECT_EQ(output.consume("U_TLAY.field0004.format"), "L1, C15, W20, H1, F1");
    }
  }

  TEST(UIC918_3_Quer_durchs_Land_Ticket, Record_0080BL)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-3 Quer-durchs-Land-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "228");

    auto const blRecord = json::parse(context->getRecord("0080BL").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-3 Quer-durchs-Land-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "52");

    EXPECT_EQ(output.consume("0080VU.terminalNummer"), "100");
    EXPECT_EQ(output.consume("0080VU.samNummer"), "0");
    EXPECT_EQ(output.consume("0080VU.anzahlPersonen"), "1");

    EXPECT_EQ(output.consume("0080VU.anzahlEfs"), "1");
    {
      EXPECT_EQ(output.consume("0080VU.efs0.berechtigungsNummer"), "665810517");
      EXPECT_EQ(output.consume("0080VU.efs0.kvpOrganisationsId"), "6260");
      EXPECT_EQ(output.consume("0080VU.efs0.pvProduktnummer"), "1201");
      EXPECT_EQ(output.consume("0080VU.efs0.pvOrganisationsId"), "6263");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigAb"), "2021-01-14T09:00:00");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigBis"), "2021-01-15T03:00:00");
      EXPECT_EQ(output.consume("0080VU.efs0.preis"), "4200");
      EXPECT_EQ(output.consume("0080VU.efs0.samSequenznummer"), "665810517");

      EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListenLaenge"), "7");
      {
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.tag"), "\xDC");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.laenge"), "5");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.typ"), "16");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.kvpOrganisationsId"), "5000");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.flaechenId"), "1");
      }
    }
  }

  TEST(UIC918_3_City_Mobil_Ticket, Metadata)
  {
    auto output = OutputConsumer{Interpreter::interpret(getData("Muster 918-3 City-Mobil Ticket.raw"))};
    EXPECT_EQ(output.size(), 34);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "285");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "391");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL 0080VU");
  }

  TEST(UIC918_3_City_Mobil_Ticket, Record_U_HEAD)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-3 City-Mobil Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context->getRecord("U_HEAD").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-3 City-Mobil Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "285");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");

    auto const blRecord = json::parse(context->getRecord("0080BL").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-3 City-Mobil Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "53");

    EXPECT_EQ(output.consume("0080VU.terminalNummer"), "100");
    EXPECT_EQ(output.consume("0080VU.samNummer"), "0");
    EXPECT_EQ(output.consume("0080VU.anzahlPersonen"), "1");

    EXPECT_EQ(output.consume("0080VU.anzahlEfs"), "1");
    {
      EXPECT_EQ(output.consume("0080VU.efs0.berechtigungsNummer"), "665654772");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigAb"), "2021-01-11T00:00:01");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigBis"), "2021-01-12T03:00:00");
      EXPECT_EQ(output.consume("0080VU.efs0.kvpOrganisationsId"), "6260");
      EXPECT_EQ(output.consume("0080VU.efs0.preis"), "600");
      EXPECT_EQ(output.consume("0080VU.efs0.pvOrganisationsId"), "6263");
      EXPECT_EQ(output.consume("0080VU.efs0.pvProduktnummer"), "1001");
      EXPECT_EQ(output.consume("0080VU.efs0.samSequenznummer"), "665654772");

      EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListenLaenge"), "8");
      {
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.tag"), "\xDC");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.laenge"), "6");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.typ"), "13");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.flaechenId"), "8003200");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.kvpOrganisationsId"), "6263");
      }
    }
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Metadata)
  {
    auto output = OutputConsumer{Interpreter::interpret(getData("Muster 918-9 Länderticket Sachsen-Anhalt.raw"))};
    EXPECT_EQ(output.size(), 55);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "1080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00001");
    EXPECT_EQ(output.consume("compressedMessageLength"), "349");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "404");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD U_TLAY U_FLEX 0080VU");
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Record_U_HEAD)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-9 Länderticket Sachsen-Anhalt.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context->getRecord("U_HEAD").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-9 Länderticket Sachsen-Anhalt.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_TLAY.recordId"), "U_TLAY");
    EXPECT_EQ(output.consume("U_TLAY.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_TLAY.recordLength"), "191");

    EXPECT_EQ(output.consume("U_TLAY.layoutStandard"), "PLAI");
    EXPECT_EQ(output.consume("U_TLAY.numberOfFields"), "8");
    {
      EXPECT_EQ(output.consume("U_TLAY.field0000.text"), "Klasse:");
      EXPECT_EQ(output.consume("U_TLAY.field0000.format"), "L3, C0, W20, H1, F0");
      EXPECT_EQ(output.consume("U_TLAY.field0005.text"), "2");
      EXPECT_EQ(output.consume("U_TLAY.field0005.format"), "L3, C19, W20, H1, F1");

      EXPECT_EQ(output.consume("U_TLAY.field0001.text"), "Pers.:");
      EXPECT_EQ(output.consume("U_TLAY.field0001.format"), "L4, C0, W20, H2, F0");
      EXPECT_EQ(output.consume("U_TLAY.field0006.text"), "1");
      EXPECT_EQ(output.consume("U_TLAY.field0006.format"), "L4, C19, W20, H2, F1");

      EXPECT_EQ(output.consume("U_TLAY.field0002.text"), "Fahrkarte");
      EXPECT_EQ(output.consume("U_TLAY.field0002.format"), "L0, C0, W80, H1, F1");
      EXPECT_EQ(output.consume("U_TLAY.field0007.text"), "Sachsen-Anhalt-Ticket");
      EXPECT_EQ(output.consume("U_TLAY.field0007.format"), "L2, C0, W80, H1, F1");

      EXPECT_EQ(output.consume("U_TLAY.field0003.text"), "Gültigkeit:");
      EXPECT_EQ(output.consume("U_TLAY.field0003.format"), "L1, C0, W20, H1, F0");
      EXPECT_EQ(output.consume("U_TLAY.field0004.text"), "18.11.2020");
      EXPECT_EQ(output.consume("U_TLAY.field0004.format"), "L1, C15, W20, H1, F1");
    }
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Record_U_FLEX)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-9 Länderticket Sachsen-Anhalt.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_FLEX.recordId"), "U_FLEX");
    EXPECT_EQ(output.consume("U_FLEX.recordVersion"), "13");
    EXPECT_EQ(output.consume("U_FLEX.recordLength"), "108");

    auto const flexRecord = json::parse(context->getRecord("U_FLEX").getJson());
    {
      EXPECT_EQ(flexRecord.size(), 2);

      auto const travelerDetail = flexRecord["travelerDetail"];
      EXPECT_EQ(travelerDetail.size(), 1);

      EXPECT_EQ(travelerDetail["travelers"].size(), 1);
      {
        auto const travelers0 = travelerDetail["travelers"][0];
        EXPECT_EQ(travelers0.size(), 3);
        EXPECT_EQ(travelers0["firstName"], "Karsten");
        EXPECT_EQ(travelers0["lastName"], "Will");
        EXPECT_EQ(travelers0["ticketHolder"], 1);
      }

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
          EXPECT_EQ(tariffs0["passengerType"], 0);
          EXPECT_EQ(tariffs0["restrictedToCountryOfResidence"], 0);
          EXPECT_EQ(tariffs0["tariffDesc"], "Sachsen-Anhalt-Ticket");
        }
      }
    }
  }

  TEST(UIC918_9_Laenderticket_Sachsen_Anhalt, Record_0080VU)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-9 Länderticket Sachsen-Anhalt.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080VU.recordId"), "0080VU");
    EXPECT_EQ(output.consume("0080VU.recordVersion"), "01");
    EXPECT_EQ(output.consume("0080VU.recordLength"), "52");

    EXPECT_EQ(output.consume("0080VU.terminalNummer"), "100");
    EXPECT_EQ(output.consume("0080VU.samNummer"), "0");
    EXPECT_EQ(output.consume("0080VU.anzahlPersonen"), "1");

    EXPECT_EQ(output.consume("0080VU.anzahlEfs"), "1");
    {
      EXPECT_EQ(output.consume("0080VU.efs0.berechtigungsNummer"), "785689900");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigAb"), "2020-11-18T00:00:00");
      EXPECT_EQ(output.consume("0080VU.efs0.gueltigBis"), "2020-11-19T03:00:00");
      EXPECT_EQ(output.consume("0080VU.efs0.kvpOrganisationsId"), "6260");
      EXPECT_EQ(output.consume("0080VU.efs0.preis"), "2400");
      EXPECT_EQ(output.consume("0080VU.efs0.pvOrganisationsId"), "6263");
      EXPECT_EQ(output.consume("0080VU.efs0.pvProduktnummer"), "1016");
      EXPECT_EQ(output.consume("0080VU.efs0.samSequenznummer"), "785689900");

      EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListenLaenge"), "7");
      {
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.tag"), "\xDC");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.laenge"), "5");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.typ"), "16");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.flaechenId"), "15");
        EXPECT_EQ(output.consume("0080VU.efs0.flaechenelementListe.kvpOrganisationsId"), "5000");
      }
    }
  }

  TEST(UIC918_9_FV_SuperSparpreis, Metadata)
  {
    auto output = OutputConsumer{Interpreter::interpret(getData("Muster 918-9 FV_SuperSparpreis.raw"))};
    EXPECT_EQ(output.size(), 10);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "02");
    EXPECT_EQ(output.consume("companyCode"), "1080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00002");
    EXPECT_EQ(output.consume("compressedMessageLength"), "199");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "188");
    EXPECT_EQ(output.consume("recordIds"), "U_FLEX");
  }

  TEST(UIC918_9_FV_SuperSparpreis, Record_U_FLEX)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-9 FV_SuperSparpreis.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_FLEX.recordId"), "U_FLEX");
    EXPECT_EQ(output.consume("U_FLEX.recordVersion"), "13");
    EXPECT_EQ(output.consume("U_FLEX.recordLength"), "188");

    auto const flexRecord = json::parse(context->getRecord("U_FLEX").getJson());
    {
      EXPECT_EQ(flexRecord.size(), 2);

      auto const travelerDetail = flexRecord["travelerDetail"];
      EXPECT_EQ(travelerDetail.size(), 1);
      {
        EXPECT_EQ(travelerDetail["travelers"].size(), 1);
        auto const travelers0 = travelerDetail["travelers"][0];
        EXPECT_EQ(travelers0.size(), 3);
        EXPECT_EQ(travelers0["firstName"], "Karsten");
        EXPECT_EQ(travelers0["lastName"], "Will");
        EXPECT_EQ(travelers0["ticketHolder"], 1);
      }

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
          EXPECT_EQ(tariffs0["passengerType"], 0);
          EXPECT_EQ(tariffs0["restrictedToCountryOfResidence"], 0);
          EXPECT_EQ(tariffs0["tariffDesc"], "Super Sparpreis");
        }
      }
    }
  }

  TEST(UIC918_3_Schleswig_Holstein_Ticket, Metadata)
  {
    auto output = OutputConsumer{Interpreter::interpret(getData("Muster 918-3 Schleswig-Holstein-Ticket.raw"))};
    EXPECT_EQ(output.size(), 55);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "351");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "531");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL U_TLAY 0080VU");
  }

  TEST(UIC918_3_Schleswig_Holstein_Ticket, Record_U_HEAD)
  {
    auto const context = Interpreter::interpret(getData("Muster 918-3 Schleswig-Holstein-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context->getRecord("U_HEAD").getJson());
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
    auto const context = Interpreter::interpret(getData("Muster 918-3 Schleswig-Holstein-Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "230");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");

    auto const blRecord = json::parse(context->getRecord("0080BL").getJson());
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

  TEST(EUR9_Ticket, Metadata)
  {
    auto output = OutputConsumer{Interpreter::interpret(getData("9EUR_Ticket.raw"))};
    EXPECT_EQ(output.size(), 55);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "336");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "507");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL U_TLAY 0080VU");
  }

  TEST(EUR9_Ticket, Record_U_HEAD)
  {
    auto const context = Interpreter::interpret(getData("9EUR_Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");

    auto const headRecord = json::parse(context->getRecord("U_HEAD").getJson());
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
    auto const context = Interpreter::interpret(getData("9EUR_Ticket.raw"));
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "218");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");

    auto const blRecord = json::parse(context->getRecord("0080BL").getJson());
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
    // output.dump();
  }

  TEST(Unknown_Ticket1, Metadata)
  {
    auto const context = Interpreter::interpret(getData("Unknown Ticket1.raw"));
    auto output = OutputConsumer{context->getFields()};
    EXPECT_EQ(output.size(), 45);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "02");
    EXPECT_EQ(output.consume("companyCode"), "1080");
    EXPECT_EQ(output.consume("signatureKeyId"), "AA006");
    EXPECT_EQ(output.consume("compressedMessageLength"), "253");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "272");
    EXPECT_EQ(output.consume("recordIds"), "U_FLEX 0080VU");
    // output.dump();
    // std::cout << json::parse(context->getRecord("U_FLEX").getJson()).dump(3) << std::endl;
  }
}
