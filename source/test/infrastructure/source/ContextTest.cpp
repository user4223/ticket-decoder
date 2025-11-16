#include <gtest/gtest.h>

#include "lib/infrastructure/include/Context.h"

#include "test/support/include/TestSupport.h"

namespace infrastructure
{
    TEST(Context, create)
    {
        auto disabler = test::support::get().disableContextTemporarily();
        auto context = Context(::utility::LoggerFactory::createLazy(false));
        EXPECT_THROW(Context(::utility::LoggerFactory::createLazy(false)), std::runtime_error);
        EXPECT_THROW(Context(), std::runtime_error);
        EXPECT_THROW(Context(::utility::LoggerFactory::createLazy(false), ::utility::DebugController()), std::runtime_error);
    }

    TEST(Context, getAllDefault)
    {
        auto disabler = test::support::get().disableContextTemporarily();
        auto context = Context();
        EXPECT_NO_THROW(context.getLoggerFactory());
        EXPECT_NO_THROW(context.getDebugController());
    }

    TEST(Context, getCustomLoggerFactory)
    {
        auto disabler = test::support::get().disableContextTemporarily();
        auto context = Context(::utility::LoggerFactory::createLazy(false));
        EXPECT_NO_THROW(context.getLoggerFactory());
        EXPECT_NO_THROW(context.getDebugController());
    }

    TEST(Context, getAllCustom)
    {
        auto disabler = test::support::get().disableContextTemporarily();
        auto debugController = ::utility::DebugController();
        debugController.define("key", {true, "k"});
        auto context = Context(::utility::LoggerFactory::createLazy(false), std::move(debugController));
        EXPECT_NO_THROW(context.getLoggerFactory());
        EXPECT_TRUE(context.getDebugController().getAs<bool>("key"));
    }
}
