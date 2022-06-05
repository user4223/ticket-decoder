
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
  EXPECT_EQ("#UT", output.at("uniqueMessageTypeId"));
  EXPECT_EQ("01", output.at("messageTypeVersion"));
  EXPECT_EQ("0080", output.at("companyCode"));
  EXPECT_EQ("00007", output.at("signatureKeyId"));
  EXPECT_EQ("0346", output.at("compressedMessageLength"));

  EXPECT_EQ("U_HEAD", output.at("U_HEAD.recordId"));
  EXPECT_EQ("01", output.at("U_HEAD.recordVersion"));
  EXPECT_EQ("53", output.at("U_HEAD.recordLength"));
  EXPECT_EQ("0080", output.at("U_HEAD.companyCode"));
  EXPECT_EQ("EZBG7S-2", output.at("U_HEAD.unambiguousTicketKey"));
  EXPECT_EQ("281020201149", output.at("U_HEAD.editionTime"));
  EXPECT_EQ("0", output.at("U_HEAD.flags"));
  EXPECT_EQ("DE", output.at("U_HEAD.editionLanguageOfTicket"));
  EXPECT_EQ("DE", output.at("U_HEAD.secondLanguageOfContract"));

  EXPECT_EQ("U_TLAY", output.at("U_TLAY.recordId"));
  EXPECT_EQ("01", output.at("U_TLAY.recordVersion"));
  EXPECT_EQ("194", output.at("U_TLAY.recordLength"));
  EXPECT_EQ("RCT2", output.at("U_TLAY.layoutStandard"));
  EXPECT_EQ("8", output.at("U_TLAY.numberOfFields"));
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
    }
}