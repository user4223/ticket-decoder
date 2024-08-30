#include <gtest/gtest.h>

#include "lib/infrastructure/include/Context.h"

#include "test/support/include/TestSupport.h"

namespace infrastructure
{
    TEST(Context, create)
    {
        test::support::uninit();
        auto context = Context(::utility::LoggerFactory::createLazy(false));
        EXPECT_THROW(Context(::utility::LoggerFactory::createLazy(false)), std::runtime_error);
        EXPECT_THROW(Context(), std::runtime_error);
        EXPECT_THROW(Context(::utility::LoggerFactory::createLazy(false), ::utility::DebugController()), std::runtime_error);
    }

    TEST(Context, getAllDefault)
    {
        test::support::uninit();
        auto context = Context();
        EXPECT_NO_THROW(context.getLoggerFactory());
        EXPECT_NO_THROW(context.getDebugController());
    }

    TEST(Context, getCustomLoggerFactory)
    {
        test::support::uninit();
        auto context = Context(::utility::LoggerFactory::createLazy(false));
        EXPECT_NO_THROW(context.getLoggerFactory());
        EXPECT_NO_THROW(context.getDebugController());
    }

    TEST(Context, getAllCustom)
    {
        test::support::uninit();
        auto context = Context(::utility::LoggerFactory::createLazy(false), ::utility::DebugController().define("key", {true, "k"}));
        EXPECT_NO_THROW(context.getLoggerFactory());
        EXPECT_TRUE(context.getDebugController().getAs<bool>("key"));
    }
}
