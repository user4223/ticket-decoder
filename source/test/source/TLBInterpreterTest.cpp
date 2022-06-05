
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
    EXPECT_EQ(output.at("0080BL.field001"), "Quer-Durchs-Land-Ticket");
    EXPECT_EQ(output.at("0080BL.field002"), "0");
    EXPECT_EQ(output.at("0080BL.field003"), "C");
    EXPECT_EQ(output.at("0080BL.field009"), "1-0-0");
    EXPECT_EQ(output.at("0080BL.field012"), "0");
    EXPECT_EQ(output.at("0080BL.field014"), "S2");
    EXPECT_EQ(output.at("0080BL.field023"), "Schrift Last");
    EXPECT_EQ(output.at("0080BL.field026"), "12");
    EXPECT_EQ(output.at("0080BL.field028"), "Last#Schrift");
    EXPECT_EQ(output.at("0080BL.field031"), "14.01.2021");
    EXPECT_EQ(output.at("0080BL.field032"), "14.01.2021");
    EXPECT_EQ(output.at("0080BL.field040"), "1");
    EXPECT_EQ(output.at("0080BL.field041"), "1");
  }
}