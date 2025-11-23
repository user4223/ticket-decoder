// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef WITH_SIGNATURE_VERIFIER

#include <gtest/gtest.h>

#include "lib/interpreter/detail/verifier/include/Certificate.h"

namespace interpreter::detail::verifier
{
  TEST(Certificate, trimTrailingNulls)
  {
    EXPECT_EQ((std::vector<std::uint8_t>{23, 42, 0, 1}), Certificate::trimTrailingNulls({23, 42, 0, 1, 0, 0}));
    EXPECT_EQ((std::vector<std::uint8_t>{0, 23, 42}), Certificate::trimTrailingNulls({0, 23, 42}));
  }

  TEST(Certificate, normalizedValidCode)
  {
    EXPECT_EQ("0050", Certificate::getNormalizedCode("50"));
    EXPECT_EQ("5000", Certificate::getNormalizedCode("5000"));
    EXPECT_EQ("0000", Certificate::getNormalizedCode("0"));
  }

  TEST(Certificate, normalizedInvalidCode)
  {
    EXPECT_THROW(Certificate::getNormalizedCode(""), std::runtime_error);
    EXPECT_THROW(Certificate::getNormalizedCode("05000"), std::runtime_error);
    EXPECT_THROW(Certificate::getNormalizedCode("0A0"), std::runtime_error);
    EXPECT_THROW(Certificate::getNormalizedCode("0.0"), std::runtime_error);
  }

  TEST(Certificate, normalizedValidId)
  {
    EXPECT_EQ("00005", Certificate::getNormalizedId("5"));
    EXPECT_EQ("0A000", Certificate::getNormalizedId("A000"));
    EXPECT_EQ("12345", Certificate::getNormalizedId("12345"));
    EXPECT_EQ("AA011", Certificate::getNormalizedId("AA011"));
  }

  TEST(Certificate, normalizedInvalidId)
  {
    EXPECT_THROW(Certificate::getNormalizedId(""), std::runtime_error);
    EXPECT_THROW(Certificate::getNormalizedId("050000"), std::runtime_error);
    EXPECT_THROW(Certificate::getNormalizedId("00-000"), std::runtime_error);
  }

  TEST(Certificate, createValidMapKey)
  {
    EXPECT_EQ("000300001", Certificate::createMapKey("3", "1"));
    EXPECT_EQ("3333ACB23", Certificate::createMapKey("3333", "ACB23"));
    EXPECT_EQ("234200777", Certificate::createMapKey("2342", "234200777"));
  }

  TEST(Certificate, createInvalidMapKey)
  {
    EXPECT_THROW(Certificate::createMapKey("3", "123456"), std::runtime_error);
    EXPECT_THROW(Certificate::createMapKey("3333", "3433ACB23"), std::runtime_error);
  }
}

#endif
