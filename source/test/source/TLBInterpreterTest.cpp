
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/uic918/include/Interpreter.h"

Interpreter::BytesType getData()
{
  auto const path = std::filesystem::path("etc").append("TestTicketPayload.raw");
  auto ifs = std::ifstream(path, std::ios::binary | std::ios::ate);
  auto const size = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  EXPECT_GT(size, 0);

  auto buffer = Interpreter::BytesType(size);
  EXPECT_TRUE(ifs.read(reinterpret_cast<char *>(buffer.data()), size));
  return buffer;
}

TEST(TLB, minimal)
{
  auto input = getData();
  auto context = Interpreter::create(Interpreter::Context{input});
  EXPECT_THAT(context.recordIds, testing::ElementsAre("U_HEAD", "0080BL", "U_TLAY", "0080VU"));

  auto output = context.output;
  EXPECT_EQ(output.at("uniqueMessageTypeId"), "#UT");
  EXPECT_EQ(output.at("messageTypeVersion"), "01");
  EXPECT_EQ(output.at("companyCode"), "0080");
  EXPECT_EQ(output.at("signatureKeyId"), "00007");
  EXPECT_EQ(output.at("compressedMessageLength"), "0346");

  EXPECT_EQ(output.at("U_HEAD.recordId"), "U_HEAD");
  EXPECT_EQ(output.at("U_HEAD.recordVersion"), "01");
  EXPECT_EQ(output.at("U_HEAD.recordLength"), "53");
  EXPECT_EQ(output.at("U_HEAD.companyCode"), "0080");
  EXPECT_EQ(output.at("U_HEAD.unambiguousTicketKey"), "EZBG7S-2");
  EXPECT_EQ(output.at("U_HEAD.editionTime"), "281020201149");
  EXPECT_EQ(output.at("U_HEAD.flags"), "0");
  EXPECT_EQ(output.at("U_HEAD.editionLanguageOfTicket"), "DE");
  EXPECT_EQ(output.at("U_HEAD.secondLanguageOfContract"), "DE");

  EXPECT_EQ(output.at("U_TLAY.recordId"), "U_TLAY");
  EXPECT_EQ(output.at("U_TLAY.recordVersion"), "01");
  EXPECT_EQ(output.at("U_TLAY.recordLength"), "194");
  EXPECT_EQ(output.at("U_TLAY.layoutStandard"), "RCT2");
  EXPECT_EQ(output.at("U_TLAY.numberOfFields"), "8");
  {
    EXPECT_EQ(output.at("U_TLAY.field0000"), "[3,0|20,1|0] Klasse:");
    EXPECT_EQ(output.at("U_TLAY.field0005"), "[3,19|20,1|1] 2");

    EXPECT_EQ(output.at("U_TLAY.field0001"), "[4,0|20,2|0] Pers.:");
    EXPECT_EQ(output.at("U_TLAY.field0006"), "[4,19|20,2|1] 1");

    EXPECT_EQ(output.at("U_TLAY.field0002"), "[0,0|80,1|1] Fahrkarte\n");
    EXPECT_EQ(output.at("U_TLAY.field0007"), "[2,0|80,1|1] QUER-DURCHS-LAND-TICKET");

    EXPECT_EQ(output.at("U_TLAY.field0003"), "[1,0|20,1|0] Gültigkeit:");
    EXPECT_EQ(output.at("U_TLAY.field0004"), "[1,15|20,1|1] 14.01.2021");
  }

  EXPECT_EQ(output.at("0080BL.ticketType"), "00");
  EXPECT_EQ(output.at("0080BL.numberOfTrips"), "1");
  {
    EXPECT_EQ(output.at("0080BL.trip0.validFrom"), "14012021");
    EXPECT_EQ(output.at("0080BL.trip0.validTo"), "14012021");
    EXPECT_EQ(output.at("0080BL.trip0.serial"), "548899912");
  }

  EXPECT_EQ(output.at("0080BL.numberOfFields"), "13");
  {
    EXPECT_EQ(output.at("0080BL.fieldS001"), "Quer-Durchs-Land-Ticket (Tarifbezeichnung)");
    EXPECT_EQ(output.at("0080BL.fieldS002"), "0 (Produktkategorie)");
    EXPECT_EQ(output.at("0080BL.fieldS003"), "C (Produktklasse Hinfahrt)");
    EXPECT_EQ(output.at("0080BL.fieldS009"), "1-0-0 (Anzahl Personen/Bahncard)");
    EXPECT_EQ(output.at("0080BL.fieldS012"), "0 (Anzahl Kinder)");
    EXPECT_EQ(output.at("0080BL.fieldS014"), "S2 (Klasse)");
    EXPECT_EQ(output.at("0080BL.fieldS023"), "Schrift Last (Inhaber)");
    EXPECT_EQ(output.at("0080BL.fieldS026"), "12 (Preisart)");
    EXPECT_EQ(output.at("0080BL.fieldS028"), "Last#Schrift (Vorname#Nachname)");
    EXPECT_EQ(output.at("0080BL.fieldS031"), "14.01.2021 (Gültig von)");
    EXPECT_EQ(output.at("0080BL.fieldS032"), "14.01.2021 (Gültig bis)");
    EXPECT_EQ(output.at("0080BL.fieldS040"), "1 (Anzahl Personen)");
    EXPECT_EQ(output.at("0080BL.fieldS041"), "1");
  }

  EXPECT_EQ(output.at("0080VU.terminalNummer"), "");
  EXPECT_EQ(output.at("0080VU.samNummer"), "");
  EXPECT_EQ(output.at("0080VU.anzahlPersonen"), "\x1");
  EXPECT_EQ(output.at("0080VU.anzahlEfs"), "\x1");
}