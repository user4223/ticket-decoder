
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <nlohmann/json.hpp>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/uic918/detail/include/Interpreter.h"
#include "lib/uic918/api/include/Record.h"

using json = nlohmann::json;
namespace uic918::detail
{
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

  std::tuple<std::string, std::string> get(nlohmann::json node, std::tuple<std::string, std::string> keys)
  {
    if (!(node.contains(std::get<0>(keys)) && node.contains(std::get<1>(keys))))
    {
      return {};
    }
    return {node[std::get<0>(keys)], node[std::get<1>(keys)]};
  }

  std::tuple<std::string, std::string> getBLField(nlohmann::json node)
  {
    return get(node, {"value", "annotation"});
  }

  TEST(Interpret, 918_3_City_Ticket)
  {
    auto const input = getData("Muster 918-3 City-Ticket.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};
    EXPECT_EQ(output.size(), 58);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "323");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "455");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL 0080VU");

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");
    EXPECT_EQ(output.consume("U_HEAD.companyCode"), "0080");
    EXPECT_EQ(output.consume("U_HEAD.uniqueTicketKey"), "F4X6XA-3");
    EXPECT_EQ(output.consume("U_HEAD.editionTime"), "2020-10-27T13:45:00");
    EXPECT_EQ(output.consume("U_HEAD.flags"), "0");
    EXPECT_EQ(output.consume("U_HEAD.editionLanguageOfTicket"), "DE");
    EXPECT_EQ(output.consume("U_HEAD.secondLanguageOfContract"), "DE");

    auto const blRecord = json::parse(context->getRecord("0080BL").getJson());
    {
      EXPECT_EQ(blRecord.size(), 2);
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

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "315");
    EXPECT_EQ(output.consume("0080BL.numberOfTrips"), "1");
    {
      EXPECT_EQ(output.consume("0080BL.trip0.validFrom"), "2021-01-13");
      EXPECT_EQ(output.consume("0080BL.trip0.validTo"), "2021-01-14");
      EXPECT_EQ(output.consume("0080BL.trip0.serial"), "548746455");
    }

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
    // output.dump();
    EXPECT_EQ(output.size(), 0);
  }

  TEST(Interpret, 918_3_Quer_durchs_Land_Ticket)
  {
    auto const input = getData("Muster 918-3 Quer-durchs-Land-Ticket.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};
    EXPECT_EQ(output.size(), 80);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "346");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "527");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL U_TLAY 0080VU");

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");
    EXPECT_EQ(output.consume("U_HEAD.companyCode"), "0080");
    EXPECT_EQ(output.consume("U_HEAD.uniqueTicketKey"), "EZBG7S-2");
    EXPECT_EQ(output.consume("U_HEAD.editionTime"), "2020-10-28T11:49:00");
    EXPECT_EQ(output.consume("U_HEAD.flags"), "0");
    EXPECT_EQ(output.consume("U_HEAD.editionLanguageOfTicket"), "DE");
    EXPECT_EQ(output.consume("U_HEAD.secondLanguageOfContract"), "DE");

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

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "228");
    EXPECT_EQ(output.consume("0080BL.ticketType"), "00");
    EXPECT_EQ(output.consume("0080BL.numberOfTrips"), "1");
    {
      EXPECT_EQ(output.consume("0080BL.trip0.validFrom"), "2021-01-14");
      EXPECT_EQ(output.consume("0080BL.trip0.validTo"), "2021-01-14");
      EXPECT_EQ(output.consume("0080BL.trip0.serial"), "548899912");
    }

    EXPECT_EQ(output.consume("0080BL.numberOfFields"), "13");
    {
      EXPECT_EQ(output.consume("0080BL.fieldS001"), "Quer-Durchs-Land-Ticket (Tarifbezeichnung)");
      EXPECT_EQ(output.consume("0080BL.fieldS002"), "0 (Produktkategorie)");
      EXPECT_EQ(output.consume("0080BL.fieldS003"), "C (Produktklasse Hinfahrt)");
      EXPECT_EQ(output.consume("0080BL.fieldS009"), "1-0-0 (Anzahl Personen/Bahncard)");
      EXPECT_EQ(output.consume("0080BL.fieldS012"), "0 (Anzahl Kinder)");
      EXPECT_EQ(output.consume("0080BL.fieldS014"), "S2 (Klasse)");
      EXPECT_EQ(output.consume("0080BL.fieldS023"), "Schrift Last (Inhaber)");
      EXPECT_EQ(output.consume("0080BL.fieldS026"), "12 (Preisart)");
      EXPECT_EQ(output.consume("0080BL.fieldS028"), "Last#Schrift (Vorname#Nachname)");
      EXPECT_EQ(output.consume("0080BL.fieldS031"), "14.01.2021 (Gültig von)");
      EXPECT_EQ(output.consume("0080BL.fieldS032"), "14.01.2021 (Gültig bis)");
      EXPECT_EQ(output.consume("0080BL.fieldS040"), "1 (Anzahl Personen)");
      EXPECT_EQ(output.consume("0080BL.fieldS041"), "1 (Anzahl EFS)");
    }

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
    // output.dump();
    EXPECT_EQ(output.size(), 0);
  }

  TEST(Interpret, 918_3_City_Mobil_Ticket)
  {
    auto const input = getData("Muster 918-3 City-Mobil Ticket.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};
    EXPECT_EQ(output.size(), 62);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "0080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00007");
    EXPECT_EQ(output.consume("compressedMessageLength"), "285");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "391");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD 0080BL 0080VU");

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");
    EXPECT_EQ(output.consume("U_HEAD.companyCode"), "0080");
    EXPECT_EQ(output.consume("U_HEAD.uniqueTicketKey"), "RPEX4F-4");
    EXPECT_EQ(output.consume("U_HEAD.editionTime"), "2020-10-27T13:18:00");
    EXPECT_EQ(output.consume("U_HEAD.flags"), "0");
    EXPECT_EQ(output.consume("U_HEAD.editionLanguageOfTicket"), "DE");
    EXPECT_EQ(output.consume("U_HEAD.secondLanguageOfContract"), "DE");

    EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
    EXPECT_EQ(output.consume("0080BL.recordLength"), "285");
    EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
    EXPECT_EQ(output.consume("0080BL.ticketType"), "03");

    EXPECT_EQ(output.consume("0080BL.numberOfTrips"), "1");
    {
      EXPECT_EQ(output.consume("0080BL.trip0.serial"), "548741714");
      EXPECT_EQ(output.consume("0080BL.trip0.validFrom"), "2021-01-11");
      EXPECT_EQ(output.consume("0080BL.trip0.validTo"), "2021-01-11");
    }

    EXPECT_EQ(output.consume("0080BL.numberOfFields"), "16");
    {
      EXPECT_EQ(output.consume("0080BL.fieldS001"), "Super Sparpreis (Tarifbezeichnung)");
      EXPECT_EQ(output.consume("0080BL.fieldS002"), "2 (Produktkategorie)");
      EXPECT_EQ(output.consume("0080BL.fieldS003"), "A (Produktklasse Hinfahrt)");
      EXPECT_EQ(output.consume("0080BL.fieldS009"), "1-1-49 (Anzahl Personen/Bahncard)");
      EXPECT_EQ(output.consume("0080BL.fieldS012"), "0 (Anzahl Kinder)");
      EXPECT_EQ(output.consume("0080BL.fieldS014"), "S2 (Klasse)");
      EXPECT_EQ(output.consume("0080BL.fieldS015"), "Mainz (Startbahnhof Hinfahrt)");
      EXPECT_EQ(output.consume("0080BL.fieldS016"), "Kassel (Zielbahnhof Hinfahrt)");
      EXPECT_EQ(output.consume("0080BL.fieldS021"), "NV*F-Flugh 14:09 ICE1196 (Wegetext)");
      EXPECT_EQ(output.consume("0080BL.fieldS023"), "Schrift Last (Inhaber)");
      EXPECT_EQ(output.consume("0080BL.fieldS026"), "13 (Preisart)");
      EXPECT_EQ(output.consume("0080BL.fieldS028"), "Last#Schrift (Vorname#Nachname)");
      EXPECT_EQ(output.consume("0080BL.fieldS031"), "11.01.2021 (Gültig von)");
      EXPECT_EQ(output.consume("0080BL.fieldS032"), "11.01.2021 (Gültig bis)");
      EXPECT_EQ(output.consume("0080BL.fieldS035"), "240 (EVA-Nummer Startbahnhof)");
      EXPECT_EQ(output.consume("0080BL.fieldS036"), "3200 (EVA-Nummer Zielbahnhof)");
    }

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
    // output.dump();
    EXPECT_EQ(output.size(), 0);
  }

  TEST(Interpret, 918_9_Laenderticket_Sachsen_Anhalt)
  {
    auto const input = getData("Muster 918-9 Länderticket Sachsen-Anhalt.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};
    EXPECT_EQ(output.size(), 61);

    EXPECT_EQ(output.consume("uniqueMessageTypeId"), "#UT");
    EXPECT_EQ(output.consume("messageTypeVersion"), "01");
    EXPECT_EQ(output.consume("companyCode"), "1080");
    EXPECT_EQ(output.consume("signatureKeyId"), "00001");
    EXPECT_EQ(output.consume("compressedMessageLength"), "349");
    EXPECT_EQ(output.consume("uncompressedMessageLength"), "404");
    EXPECT_EQ(output.consume("recordIds"), "U_HEAD U_TLAY U_FLEX 0080VU");

    EXPECT_EQ(output.consume("U_HEAD.recordId"), "U_HEAD");
    EXPECT_EQ(output.consume("U_HEAD.recordVersion"), "01");
    EXPECT_EQ(output.consume("U_HEAD.recordLength"), "53");
    EXPECT_EQ(output.consume("U_HEAD.companyCode"), "1080");
    EXPECT_EQ(output.consume("U_HEAD.uniqueTicketKey"), "61B3JR37");
    EXPECT_EQ(output.consume("U_HEAD.editionTime"), "2020-11-09T15:17:00");
    EXPECT_EQ(output.consume("U_HEAD.flags"), "0");
    EXPECT_EQ(output.consume("U_HEAD.editionLanguageOfTicket"), "DE");
    EXPECT_EQ(output.consume("U_HEAD.secondLanguageOfContract"), "DE");

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

    EXPECT_EQ(output.consume("U_FLEX.recordId"), "U_FLEX");
    EXPECT_EQ(output.consume("U_FLEX.recordVersion"), "13");
    EXPECT_EQ(output.consume("U_FLEX.recordLength"), "108");

    auto const flexRecord = json::parse(context->getRecord("U_FLEX").getJson());
    {
      EXPECT_EQ(flexRecord.size(), 2);

      auto const travelerDetail = flexRecord["travelerDetail"];
      EXPECT_EQ(travelerDetail.size(), 1);

      EXPECT_EQ(travelerDetail["travelers"].size(), 1);
      auto const travelers0 = travelerDetail["travelers"][0];
      EXPECT_EQ(travelers0.size(), 2);
      EXPECT_EQ(travelers0["firstName"], "Karsten");
      EXPECT_EQ(travelers0["lastName"], "Will");

      auto const transportDocuments = flexRecord["transportDocuments"];
      EXPECT_EQ(transportDocuments.size(), 1);
      EXPECT_EQ(transportDocuments[0].size(), 1);
      auto const openTicket0 = transportDocuments[0]["openTicket"];
      EXPECT_EQ(openTicket0.size(), 3);
      EXPECT_EQ(openTicket0["referenceIA5"], "FTJ9KNEM");
      EXPECT_EQ(openTicket0["classCode"], "2");

      EXPECT_EQ(openTicket0["tariffs"].size(), 1);
      auto const tariffs0 = openTicket0["tariffs"][0];
      EXPECT_EQ(tariffs0.size(), 2);
      EXPECT_EQ(tariffs0["numberOfPassengers"], 1);
      EXPECT_EQ(tariffs0["tariffDesc"], "Sachsen-Anhalt-Ticket");
    }
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
    // output.dump();
    EXPECT_EQ(output.size(), 0);
  }

  TEST(Interpret, 918_9_FV_SuperSparpreis)
  {
    auto const input = getData("Muster 918-9 FV_SuperSparpreis.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.size(), 10);
    // output.dump();
    // std::cout << json::parse(context->getRecord("U_FLEX").getJson()).dump(3) << std::endl;
  }

  TEST(Interpret, 918_3_Schleswig_Holstein_Ticket)
  {
    auto const input = getData("Muster 918-3 Schleswig-Holstein-Ticket.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.size(), 80);
    // output.dump();
  }

  TEST(Interpret, EUR9_Ticket)
  {
    auto const input = getData("9EUR_Ticket.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.size(), 80);
    // output.dump();
  }

  TEST(Interpret, Unknown1_Ticket)
  {
    auto const input = getData("Unknown Ticket1.raw");
    auto const context = Interpreter::interpret(input);
    auto output = OutputConsumer{context->getFields()};

    EXPECT_EQ(output.size(), 45);
    // output.dump();
    // std::cout << json::parse(context->getRecord("U_FLEX").getJson()).dump(3) << std::endl;
  }
}
