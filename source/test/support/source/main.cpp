// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "gtest/gtest.h"

#include "../include/TestSupport.h"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  auto const testSupport = ::test::support::TestSupport(argc, argv);
  return RUN_ALL_TESTS();
}