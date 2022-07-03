
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/uic918/include/Interpreter.h"

Context::BytesType getData(std::string fileName)
{
  auto const path = std::filesystem::path("..").append("etc").append(fileName);
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

  std::string consume(std::string key)
  {
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

TEST(Interpret, 918_3_City_Ticket)
{
  auto const input = getData("Muster 918-3 City-Ticket.raw");
  auto output = OutputConsumer{Interpreter::interpretRaw(input)};
  EXPECT_EQ(output.size(), 76);

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

  EXPECT_EQ(output.consume("0080BL.recordId"), "0080BL");
  EXPECT_EQ(output.consume("0080BL.recordVersion"), "03");
  EXPECT_EQ(output.consume("0080BL.recordLength"), "315");
  EXPECT_EQ(output.consume("0080BL.ticketType"), "02");
  EXPECT_EQ(output.consume("0080BL.numberOfTrips"), "1");
  {
    EXPECT_EQ(output.consume("0080BL.trip0.validFrom"), "2021-01-13");
    EXPECT_EQ(output.consume("0080BL.trip0.validTo"), "2021-01-14");
    EXPECT_EQ(output.consume("0080BL.trip0.serial"), "548746455");
  }

  EXPECT_EQ(output.consume("0080BL.numberOfFields"), "16");
  {
    EXPECT_EQ(output.consume("0080BL.fieldS001"), "Flexpreis (Tarifbezeichnung)");
    EXPECT_EQ(output.consume("0080BL.fieldS002"), "2 (Produktkategorie)");
    EXPECT_EQ(output.consume("0080BL.fieldS003"), "A (Produktklasse Hinfahrt)");
    EXPECT_EQ(output.consume("0080BL.fieldS009"), "1-0-0 (Anzahl Personen/Bahncard)");
    EXPECT_EQ(output.consume("0080BL.fieldS012"), "0 (Anzahl Kinder)");
    EXPECT_EQ(output.consume("0080BL.fieldS014"), "S2 (Klasse)");
    EXPECT_EQ(output.consume("0080BL.fieldS015"), "Kassel+City (Startbahnhof Hinfahrt)");
    EXPECT_EQ(output.consume("0080BL.fieldS016"), "Frankfurt(Main)+City (Zielbahnhof Hinfahrt)");
    EXPECT_EQ(output.consume("0080BL.fieldS021"), "VIA: GUNT*(BEB*HERS*FD*GELN/COEL*LOLL*GI) (Wegetext)");
    EXPECT_EQ(output.consume("0080BL.fieldS023"), "Schrift Last (Inhaber)");
    EXPECT_EQ(output.consume("0080BL.fieldS026"), "12 (Preisart)");
    EXPECT_EQ(output.consume("0080BL.fieldS028"), "Last#Schrift (Vorname#Nachname)");
    EXPECT_EQ(output.consume("0080BL.fieldS031"), "13.01.2021 (Gültig von)");
    EXPECT_EQ(output.consume("0080BL.fieldS032"), "14.01.2021 (Gültig bis)");
    EXPECT_EQ(output.consume("0080BL.fieldS035"), "3200 (EVA-Nummer Startbahnhof)");
    EXPECT_EQ(output.consume("0080BL.fieldS036"), "105 (EVA-Nummer Zielbahnhof)");
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
  output.dump(); // TODO Check all remaining fields explicitly
  EXPECT_EQ(output.size(), 0);
}

TEST(Interpret, 918_3_Quer_durchs_Land_Ticket)
{
  auto const input = getData("Muster 918-3 Quer-durchs-Land-Ticket.raw");
  auto output = OutputConsumer{Interpreter::interpretRaw(input)};
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
  output.dump(); // TODO Check all remaining fields explicitly
  EXPECT_EQ(output.size(), 0);
}

TEST(Interpret, 918_3_City_Mobil_Ticket)
{
  auto const input = getData("Muster 918-3 City-Mobil Ticket.raw");
  auto output = OutputConsumer{Interpreter::interpretRaw(input)};
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
  output.dump(); // TODO Check all remaining fields explicitly
  EXPECT_EQ(output.size(), 0);
}
