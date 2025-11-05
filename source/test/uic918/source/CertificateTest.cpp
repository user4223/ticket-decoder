#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/UicCertificate.h"

namespace uic918::detail
{

  TEST(Certificate, trimTrailingNulls)
  {
    EXPECT_EQ((std::vector<std::uint8_t>{23, 42, 0, 1}), UicCertificate::trimTrailingNulls({23, 42, 0, 1, 0, 0}));
    EXPECT_EQ((std::vector<std::uint8_t>{0, 23, 42}), UicCertificate::trimTrailingNulls({0, 23, 42}));
  }

  TEST(Certificate, normalizedValidCode)
  {
    EXPECT_EQ("0050", UicCertificate::getNormalizedCode("50"));
    EXPECT_EQ("5000", UicCertificate::getNormalizedCode("5000"));
    EXPECT_EQ("0000", UicCertificate::getNormalizedCode("0"));
  }

  TEST(Certificate, normalizedInvalidCode)
  {
    EXPECT_THROW(UicCertificate::getNormalizedCode(""), std::runtime_error);
    EXPECT_THROW(UicCertificate::getNormalizedCode("05000"), std::runtime_error);
    EXPECT_THROW(UicCertificate::getNormalizedCode("0A0"), std::runtime_error);
    EXPECT_THROW(UicCertificate::getNormalizedCode("0.0"), std::runtime_error);
  }

  TEST(Certificate, normalizedValidId)
  {
    EXPECT_EQ("00005", UicCertificate::getNormalizedId("5"));
    EXPECT_EQ("0A000", UicCertificate::getNormalizedId("A000"));
    EXPECT_EQ("12345", UicCertificate::getNormalizedId("12345"));
    EXPECT_EQ("AA011", UicCertificate::getNormalizedId("AA011"));
  }

  TEST(Certificate, normalizedInvalidId)
  {
    EXPECT_THROW(UicCertificate::getNormalizedId(""), std::runtime_error);
    EXPECT_THROW(UicCertificate::getNormalizedId("050000"), std::runtime_error);
    EXPECT_THROW(UicCertificate::getNormalizedId("00-000"), std::runtime_error);
  }

  TEST(Certificate, createValidMapKey)
  {
    EXPECT_EQ("000300001", UicCertificate::createMapKey("3", "1"));
    EXPECT_EQ("3333ACB23", UicCertificate::createMapKey("3333", "ACB23"));
    EXPECT_EQ("234200777", UicCertificate::createMapKey("2342", "234200777"));
  }

  TEST(Certificate, createInvalidMapKey)
  {
    EXPECT_THROW(UicCertificate::createMapKey("3", "123456"), std::runtime_error);
    EXPECT_THROW(UicCertificate::createMapKey("3333", "3433ACB23"), std::runtime_error);
  }
}
