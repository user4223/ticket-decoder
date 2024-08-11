
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test/support/include/TestSupport.h"

#include "lib/io/api/include/Utility.h"

namespace io::api
{
    auto base = []()
    { return ::test::support::getExecutableFolderPath() / "etc" / "io"; };

    TEST(Utility, areFiles)
    {
        EXPECT_TRUE(utility::areFiles({base() / "minimal.png"}));
        EXPECT_TRUE(utility::areFiles({base() / "minimal.png", base() / "minimal.jpg"}));
    }

    TEST(Utility, areNoFiles)
    {
        EXPECT_FALSE(utility::areFiles({}));
        EXPECT_FALSE(utility::areFiles({base()}));
        EXPECT_FALSE(utility::areFiles({base(), base().parent_path()}));
        EXPECT_FALSE(utility::areFiles({base(), base() / "minimal.png"}));
        EXPECT_FALSE(utility::areFiles({base() / "minimal.png", base()}));
        EXPECT_FALSE(utility::areFiles({base() / "not_existing_79237459723495345jljlwjerlwjejrl.pdf"}));
    }

    TEST(Utility, areDirectories)
    {
        EXPECT_TRUE(utility::areDirectories({std::filesystem::current_path()}));
        EXPECT_TRUE(utility::areDirectories({base()}));
        EXPECT_TRUE(utility::areDirectories({base(), base().parent_path()}));
    }

    TEST(Utility, areNoDirectories)
    {
        EXPECT_FALSE(utility::areDirectories({}));
        EXPECT_FALSE(utility::areDirectories({base() / "minimal.png"}));
        EXPECT_FALSE(utility::areDirectories({base() / "minimal.png", base() / "minimal.jpg"}));
        EXPECT_FALSE(utility::areDirectories({base(), base() / "minimal.png"}));
        EXPECT_FALSE(utility::areDirectories({base() / "minimal.png", base()}));
        EXPECT_FALSE(utility::areDirectories({base() / "not_existing_282345793479274359ljlsdjflsdf"}));
    }
}
