
#include <gtest/gtest.h>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/uic918-3/include/Interpreter.h"

std::vector<std::uint8_t> getData()
{
  auto const path = std::filesystem::path("etc").append("TestTicketPayload.raw");
  auto ifs = std::ifstream(path, std::ios::binary | std::ios::ate);
  auto const size = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  EXPECT_GT(size, 0);

  auto buffer = std::vector<std::uint8_t>(size);
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
}