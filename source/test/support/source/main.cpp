#include "gtest/gtest.h"

#include "../include/TestSupport.h"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  auto const testSupport = ::test::support::TestSupport(argc, argv);
  return RUN_ALL_TESTS();
}