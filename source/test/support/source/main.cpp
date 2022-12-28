#include "gtest/gtest.h"

#include "../include/Loader.h"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::support::Loader::init(argc, argv);
  return RUN_ALL_TESTS();
}