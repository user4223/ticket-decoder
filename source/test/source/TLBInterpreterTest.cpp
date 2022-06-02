
#include <gtest/gtest.h>

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
  auto output = Interpreter::create(input);

  // EXPECT_EQ({"U_HEAD","U_TLAY"}, output.recordIds);

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

  EXPECT_EQ("Klasse:", output.at("U_TLAY.field0000"));
  EXPECT_EQ("Pers.:", output.at("U_TLAY.field0001"));
  EXPECT_EQ("Fahrkarte\n", output.at("U_TLAY.field0002"));
  EXPECT_EQ("Gültigkeit:", output.at("U_TLAY.field0003"));
  EXPECT_EQ("14.01.2021", output.at("U_TLAY.field0004"));
  EXPECT_EQ("2", output.at("U_TLAY.field0005"));
  EXPECT_EQ("1", output.at("U_TLAY.field0006"));
  EXPECT_EQ("QUER-DURCHS-LAND-TICKET", output.at("U_TLAY.field0007"));
}