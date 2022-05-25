
#include <gtest/gtest.h>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/uic918-3/include/Interpreter.h"

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

TEST(TLB1, minimal)
{
  auto input = getData();
  auto output = Interpreter::create(input);

  EXPECT_EQ("#UT", output.at("uniqueMessageTypeId"));
  EXPECT_EQ("01", output.at("messageTypeVersion"));
  EXPECT_EQ("0080", output.at("companyCode"));
  EXPECT_EQ("00007", output.at("signatureKeyId"));
  EXPECT_EQ("0346", output.at("compressedMessageLength"));

  EXPECT_EQ("U_HEAD", output.at("mainRecord.recordId"));
  EXPECT_EQ("01", output.at("mainRecord.recordVersion"));
  EXPECT_EQ("0053", output.at("mainRecord.companyCode"));
  EXPECT_EQ("0080EZBG7S-2", output.at("mainRecord.unambiguousTicketKey"));
}