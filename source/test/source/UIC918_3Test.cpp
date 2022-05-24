
#include <gtest/gtest.h>

#include <vector>
#include <filesystem>
#include <fstream>

#include "lib/include/UIC918_3Interpreter.h"

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

TEST(UIC918_3, minimal)
{
  auto const buffer = getData();
}