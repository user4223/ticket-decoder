#include <gtest/gtest.h>

#include "lib/utility/include/DebugController.h"

namespace utility
{

    TEST(DebugController, getUndefined)
    {
        auto controller = DebugController();
        EXPECT_EQ(4, controller.getAs<int>("setting", 4));
        EXPECT_EQ(-1, controller.getAs<int>("setting", -1));
        EXPECT_EQ(4u, controller.getAs<unsigned int>("setting", 4u));
        EXPECT_EQ(0u, controller.getAs<unsigned int>("setting", 0u));
        EXPECT_EQ(false, controller.getAs<bool>("setting", false));
        EXPECT_EQ(true, controller.getAs<bool>("setting", true));
    }

    TEST(DebugController, incrementUnsignedInteger)
    {
        auto controller = DebugController();
        EXPECT_THROW(controller.incrementAs<unsigned int>("setting"), std::runtime_error);

        controller.define("setting", {0u, 0u, 2u, "s"});
        EXPECT_EQ(1u, controller.incrementAs<unsigned int>("setting"));
        EXPECT_EQ(2u, controller.incrementAs<unsigned int>("setting"));
        EXPECT_EQ(2u, controller.incrementAs<unsigned int>("setting"));
    }

    TEST(DebugController, decrementUnsignedInteger)
    {
        auto controller = DebugController();
        EXPECT_THROW(controller.decrementAs<unsigned int>("setting"), std::runtime_error);

        controller.define("setting", {0u, 2u, 2u, "s"});
        EXPECT_EQ(1u, controller.decrementAs<unsigned int>("setting"));
        EXPECT_EQ(0u, controller.decrementAs<unsigned int>("setting"));
        EXPECT_EQ(0u, controller.decrementAs<unsigned int>("setting"));
    }

    TEST(DebugController, rotateInteger)
    {
        auto controller = DebugController();
        EXPECT_THROW(controller.incrementRotateAs<int>("setting", 1), std::runtime_error);
        EXPECT_THROW(controller.incrementRotateAs<int>("setting", 2), std::runtime_error);

        controller.define("setting", {0, 1, 2, "s"});
        EXPECT_EQ(2, controller.incrementRotateAs<int>("setting", 1));
        EXPECT_EQ(0, controller.incrementRotateAs<int>("setting", 1));
        EXPECT_EQ(1, controller.incrementRotateAs<int>("setting", 1));

        EXPECT_EQ(0, controller.incrementRotateAs<int>("setting", -1));
        EXPECT_EQ(2, controller.incrementRotateAs<int>("setting", -1));
        EXPECT_EQ(1, controller.incrementRotateAs<int>("setting", -1));

        EXPECT_EQ(0, controller.incrementRotateAs<int>("setting", 2));
        EXPECT_EQ(2, controller.incrementRotateAs<int>("setting", 2));
        EXPECT_EQ(0, controller.incrementRotateAs<int>("setting", 2));
    }

    TEST(DebugController, toggleBool)
    {
        auto controller = DebugController();
        EXPECT_THROW(controller.toggle("setting"), std::runtime_error);

        controller.define("setting", {true, "s"});
        EXPECT_EQ(false, controller.toggle("setting"));
        EXPECT_EQ(true, controller.toggle("setting"));
    }

    TEST(DebugController, touched)
    {
        auto controller = DebugController();
        EXPECT_FALSE(controller.touched());
        controller.define("setting", {true, "s"});
        EXPECT_TRUE(controller.touched());
        controller.getAs<bool>("setting", false);
        EXPECT_TRUE(controller.touched());
        controller.getAs<bool>("setting", false);
        EXPECT_FALSE(controller.touched());
        EXPECT_FALSE(controller.touched());
        controller.toggle("setting");
        EXPECT_TRUE(controller.touched());
        controller.getAs<bool>("setting", false);
        EXPECT_TRUE(controller.touched());
        controller.getAs<bool>("setting", false);
        EXPECT_FALSE(controller.touched());
    }
}
