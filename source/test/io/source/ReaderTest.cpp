
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/Reader.h"

#include "test/support/include/TestSupport.h"

namespace io::api
{
    auto const ioEtc = []()
    { return ::test::support::getExecutableFolderPath() / "etc" / "io"; };

    TEST(ReaderValidate, pathDoesNotExists)
    {
        EXPECT_THROW(Reader::validate(ioEtc() / "does_not_exists.png", {".png"}), std::runtime_error);
    }

    TEST(ReaderValidate, pathExistsButNoRegularFile)
    {
        EXPECT_THROW(Reader::validate(ioEtc(), {".png"}), std::runtime_error);
    }

    TEST(ReaderValidate, pathExistsButUnexpectedExtension)
    {
        EXPECT_THROW(Reader::validate(ioEtc() / "minimal.png", {".jpg"}), std::runtime_error);
    }

    TEST(ReaderValidate, pathExistsAndIsMatchingExtension)
    {
        Reader::validate(ioEtc() / "minimal.png", {".jpg", ".png"});
        Reader::validate(ioEtc() / "minimal.png", {".bla", ".jpg", ".png"});
        Reader::validate(ioEtc() / "minimal.jpg", {".jpg", ".png"});
        Reader::validate(ioEtc() / "minimal.jpg", {".foot", ".jpg", ".png"});
    }

    TEST(Reader, normalizeExtension)
    {
        EXPECT_EQ(".pdf", Reader::normalizeExtension(std::filesystem::path("bla") / "foo.pdf"));
        EXPECT_EQ(".pdf", Reader::normalizeExtension(std::filesystem::path("bla") / "foo.PDF"));
        EXPECT_EQ(".pdf", Reader::normalizeExtension(std::filesystem::path("bla") / "foo.pDf"));
        EXPECT_EQ(".blubber", Reader::normalizeExtension(std::filesystem::path("bla") / "foo.bluBBer"));
    }

    TEST(Reader, normalizeInvalidExtension)
    {
        EXPECT_EQ("", Reader::normalizeExtension(std::filesystem::path("bla") / ".foo"));
        EXPECT_EQ("", Reader::normalizeExtension(std::filesystem::path("bla") / "."));
        EXPECT_EQ("", Reader::normalizeExtension(std::filesystem::path("bla/")));
    }
}
