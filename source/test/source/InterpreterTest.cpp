
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

TEST(Interpret, 918_3_City_Ticket)
{
  auto const input = getData("Muster 918-3 City-Ticket.raw");
  auto const output = Interpreter::interpret(input);

  EXPECT_EQ(output.at("uniqueMessageTypeId").toString(), "#UT");
  EXPECT_EQ(output.at("messageTypeVersion").toString(), "01");
  EXPECT_EQ(output.at("companyCode").toString(), "0080");
  EXPECT_EQ(output.at("signatureKeyId").toString(), "00007");
  EXPECT_EQ(output.at("compressedMessageLength").toString(), "323");
  EXPECT_EQ(output.at("uncompressedMessageLength").toString(), "455");
  EXPECT_EQ(output.at("recordIds").toString(), "U_HEAD 0080BL 0080VU");

  EXPECT_EQ(output.at("U_HEAD.recordId").toString(), "U_HEAD");
  EXPECT_EQ(output.at("U_HEAD.recordVersion").toString(), "01");
  EXPECT_EQ(output.at("U_HEAD.recordLength").toString(), "53");
  EXPECT_EQ(output.at("U_HEAD.companyCode").toString(), "0080");
  EXPECT_EQ(output.at("U_HEAD.uniqueTicketKey").toString(), "F4X6XA-3");
  EXPECT_EQ(output.at("U_HEAD.editionTime").toString(), "271020201345");
  EXPECT_EQ(output.at("U_HEAD.flags").toString(), "0");
  EXPECT_EQ(output.at("U_HEAD.editionLanguageOfTicket").toString(), "DE");
  EXPECT_EQ(output.at("U_HEAD.secondLanguageOfContract").toString(), "DE");

  EXPECT_EQ(output.at("0080BL.recordId").toString(), "0080BL");
  EXPECT_EQ(output.at("0080BL.recordVersion").toString(), "03");
  EXPECT_EQ(output.at("0080BL.recordLength").toString(), "315");
  EXPECT_EQ(output.at("0080BL.ticketType").toString(), "02");
  EXPECT_EQ(output.at("0080BL.numberOfTrips").toString(), "1");
  {
    EXPECT_EQ(output.at("0080BL.trip0.validFrom").toString(), "13012021");
    EXPECT_EQ(output.at("0080BL.trip0.validTo").toString(), "14012021");
    EXPECT_EQ(output.at("0080BL.trip0.serial").toString(), "548746455");
  }

  EXPECT_EQ(output.at("0080BL.numberOfFields").toString(), "16");
  {
    EXPECT_EQ(output.at("0080BL.fieldS001").toString(), "Flexpreis (Tarifbezeichnung)");
    EXPECT_EQ(output.at("0080BL.fieldS002").toString(), "2 (Produktkategorie)");
    EXPECT_EQ(output.at("0080BL.fieldS003").toString(), "A (Produktklasse Hinfahrt)");
    EXPECT_EQ(output.at("0080BL.fieldS009").toString(), "1-0-0 (Anzahl Personen/Bahncard)");
    EXPECT_EQ(output.at("0080BL.fieldS012").toString(), "0 (Anzahl Kinder)");
    EXPECT_EQ(output.at("0080BL.fieldS014").toString(), "S2 (Klasse)");
    EXPECT_EQ(output.at("0080BL.fieldS015").toString(), "Kassel+City (Startbahnhof Hinfahrt)");
    EXPECT_EQ(output.at("0080BL.fieldS016").toString(), "Frankfurt(Main)+City (Zielbahnhof Hinfahrt)");
    EXPECT_EQ(output.at("0080BL.fieldS021").toString(), "VIA: GUNT*(BEB*HERS*FD*GELN/COEL*LOLL*GI) (Wegetext)");
    EXPECT_EQ(output.at("0080BL.fieldS023").toString(), "Schrift Last (Inhaber)");
    EXPECT_EQ(output.at("0080BL.fieldS026").toString(), "12 (Preisart)");
    EXPECT_EQ(output.at("0080BL.fieldS028").toString(), "Last#Schrift (Vorname#Nachname)");
    EXPECT_EQ(output.at("0080BL.fieldS031").toString(), "13.01.2021 (Gültig von)");
    EXPECT_EQ(output.at("0080BL.fieldS032").toString(), "14.01.2021 (Gültig bis)");
    EXPECT_EQ(output.at("0080BL.fieldS035").toString(), "3200 (EVA-Nummer Startbahnhof)");
    EXPECT_EQ(output.at("0080BL.fieldS036").toString(), "105 (EVA-Nummer Zielbahnhof)");
  }
}

TEST(Interpret, 918_3_Quer_durchs_Land_Ticket)
{
  auto const input = getData("Muster 918-3 Quer-durchs-Land-Ticket.raw");
  auto const output = Interpreter::interpret(input);

  EXPECT_EQ(output.at("uniqueMessageTypeId").toString(), "#UT");
  EXPECT_EQ(output.at("messageTypeVersion").toString(), "01");
  EXPECT_EQ(output.at("companyCode").toString(), "0080");
  EXPECT_EQ(output.at("signatureKeyId").toString(), "00007");
  EXPECT_EQ(output.at("compressedMessageLength").toString(), "346");
  EXPECT_EQ(output.at("uncompressedMessageLength").toString(), "527");
  EXPECT_EQ(output.at("recordIds").toString(), "U_HEAD 0080BL U_TLAY 0080VU");

  EXPECT_EQ(output.at("U_HEAD.recordId").toString(), "U_HEAD");
  EXPECT_EQ(output.at("U_HEAD.recordVersion").toString(), "01");
  EXPECT_EQ(output.at("U_HEAD.recordLength").toString(), "53");
  EXPECT_EQ(output.at("U_HEAD.companyCode").toString(), "0080");
  EXPECT_EQ(output.at("U_HEAD.uniqueTicketKey").toString(), "EZBG7S-2");
  EXPECT_EQ(output.at("U_HEAD.editionTime").toString(), "281020201149");
  EXPECT_EQ(output.at("U_HEAD.flags").toString(), "0");
  EXPECT_EQ(output.at("U_HEAD.editionLanguageOfTicket").toString(), "DE");
  EXPECT_EQ(output.at("U_HEAD.secondLanguageOfContract").toString(), "DE");

  EXPECT_EQ(output.at("U_TLAY.recordId").toString(), "U_TLAY");
  EXPECT_EQ(output.at("U_TLAY.recordVersion").toString(), "01");
  EXPECT_EQ(output.at("U_TLAY.recordLength").toString(), "194");
  EXPECT_EQ(output.at("U_TLAY.layoutStandard").toString(), "RCT2");
  EXPECT_EQ(output.at("U_TLAY.numberOfFields").toString(), "8");
  {
    EXPECT_EQ(output.at("U_TLAY.field0000.text").toString(), "Klasse:");
    EXPECT_EQ(output.at("U_TLAY.field0000.format").toString(), "L3, C0, W20, H1, F0");
    EXPECT_EQ(output.at("U_TLAY.field0005.text").toString(), "2");
    EXPECT_EQ(output.at("U_TLAY.field0005.format").toString(), "L3, C19, W20, H1, F1");

    EXPECT_EQ(output.at("U_TLAY.field0001.text").toString(), "Pers.:");
    EXPECT_EQ(output.at("U_TLAY.field0001.format").toString(), "L4, C0, W20, H2, F0");
    EXPECT_EQ(output.at("U_TLAY.field0006.text").toString(), "1");
    EXPECT_EQ(output.at("U_TLAY.field0006.format").toString(), "L4, C19, W20, H2, F1");

    EXPECT_EQ(output.at("U_TLAY.field0002.text").toString(), "Fahrkarte\n");
    EXPECT_EQ(output.at("U_TLAY.field0002.format").toString(), "L0, C0, W80, H1, F1");
    EXPECT_EQ(output.at("U_TLAY.field0007.text").toString(), "QUER-DURCHS-LAND-TICKET");
    EXPECT_EQ(output.at("U_TLAY.field0007.format").toString(), "L2, C0, W80, H1, F1");

    EXPECT_EQ(output.at("U_TLAY.field0003.text").toString(), "Gültigkeit:");
    EXPECT_EQ(output.at("U_TLAY.field0003.format").toString(), "L1, C0, W20, H1, F0");
    EXPECT_EQ(output.at("U_TLAY.field0004.text").toString(), "14.01.2021");
    EXPECT_EQ(output.at("U_TLAY.field0004.format").toString(), "L1, C15, W20, H1, F1");
  }

  EXPECT_EQ(output.at("0080BL.recordId").toString(), "0080BL");
  EXPECT_EQ(output.at("0080BL.recordVersion").toString(), "03");
  EXPECT_EQ(output.at("0080BL.recordLength").toString(), "228");
  EXPECT_EQ(output.at("0080BL.ticketType").toString(), "00");
  EXPECT_EQ(output.at("0080BL.numberOfTrips").toString(), "1");
  {
    EXPECT_EQ(output.at("0080BL.trip0.validFrom").toString(), "14012021");
    EXPECT_EQ(output.at("0080BL.trip0.validTo").toString(), "14012021");
    EXPECT_EQ(output.at("0080BL.trip0.serial").toString(), "548899912");
  }

  EXPECT_EQ(output.at("0080BL.numberOfFields").toString(), "13");
  {
    EXPECT_EQ(output.at("0080BL.fieldS001").toString(), "Quer-Durchs-Land-Ticket (Tarifbezeichnung)");
    EXPECT_EQ(output.at("0080BL.fieldS002").toString(), "0 (Produktkategorie)");
    EXPECT_EQ(output.at("0080BL.fieldS003").toString(), "C (Produktklasse Hinfahrt)");
    EXPECT_EQ(output.at("0080BL.fieldS009").toString(), "1-0-0 (Anzahl Personen/Bahncard)");
    EXPECT_EQ(output.at("0080BL.fieldS012").toString(), "0 (Anzahl Kinder)");
    EXPECT_EQ(output.at("0080BL.fieldS014").toString(), "S2 (Klasse)");
    EXPECT_EQ(output.at("0080BL.fieldS023").toString(), "Schrift Last (Inhaber)");
    EXPECT_EQ(output.at("0080BL.fieldS026").toString(), "12 (Preisart)");
    EXPECT_EQ(output.at("0080BL.fieldS028").toString(), "Last#Schrift (Vorname#Nachname)");
    EXPECT_EQ(output.at("0080BL.fieldS031").toString(), "14.01.2021 (Gültig von)");
    EXPECT_EQ(output.at("0080BL.fieldS032").toString(), "14.01.2021 (Gültig bis)");
    EXPECT_EQ(output.at("0080BL.fieldS040").toString(), "1 (Anzahl Personen)");
    EXPECT_EQ(output.at("0080BL.fieldS041").toString(), "1 (Anzahl EFS)");
  }

  EXPECT_EQ(output.at("0080VU.recordId").toString(), "0080VU");
  EXPECT_EQ(output.at("0080VU.recordVersion").toString(), "01");
  EXPECT_EQ(output.at("0080VU.recordLength").toString(), "52");
  EXPECT_EQ(output.at("0080VU.terminalNummer").toString(), "100");
  EXPECT_EQ(output.at("0080VU.samNummer").toString(), "0");
  EXPECT_EQ(output.at("0080VU.anzahlPersonen").toString(), "1");

  EXPECT_EQ(output.at("0080VU.anzahlEfs").toString(), "1");
  {
    EXPECT_EQ(output.at("0080VU.efs0.berechtigungsNummer").toString(), "665810517");
    EXPECT_EQ(output.at("0080VU.efs0.kvpOrganisationsId").toString(), "6260");
    EXPECT_EQ(output.at("0080VU.efs0.pvProduktnummer").toString(), "1201");
    EXPECT_EQ(output.at("0080VU.efs0.pvOrganisationsId").toString(), "6263");
    EXPECT_EQ(output.at("0080VU.efs0.gueltigAb").toString(), "2021-01-14T09:00:00");
    EXPECT_EQ(output.at("0080VU.efs0.gueltigBis").toString(), "2021-01-15T03:00:00");
    EXPECT_EQ(output.at("0080VU.efs0.preis").toString(), "4200");
    EXPECT_EQ(output.at("0080VU.efs0.samSequenznummer").toString(), "665810517");
    EXPECT_EQ(output.at("0080VU.efs0.flaechenelementListenLaenge").toString(), "7");
    EXPECT_EQ(output.at("0080VU.efs0.flaechenelementListe.tag").toString(), "\xDC");
    EXPECT_EQ(output.at("0080VU.efs0.flaechenelementListe.laenge").toString(), "5");
    EXPECT_EQ(output.at("0080VU.efs0.flaechenelementListe.typ").toString(), "16");
    EXPECT_EQ(output.at("0080VU.efs0.flaechenelementListe.kvpOrganisationsId").toString(), "5000");
    EXPECT_EQ(output.at("0080VU.efs0.flaechenelementListe.flaechenId").toString(), "1");
  }
}