
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"

#include "test/support/include/Loader.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    auto const ioEtc = []()
    { return support::Loader::getExecutableFolderPath() / "etc" / "io"; };

    TEST(Loader, directory)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc());
        EXPECT_EQ(3, elements.size());
    }

    TEST(Loader, imageFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "minimal.jpg");
        EXPECT_EQ(1, elements.size());
    }

    TEST(Loader, pdfFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "minimal.pdf");
        EXPECT_EQ(1, elements.size());
    }

    TEST(Loader, notExisting)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "crappy.jpg");
        EXPECT_EQ(0, elements.size());
    }
}
