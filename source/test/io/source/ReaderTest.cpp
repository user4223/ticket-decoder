
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/Reader.h"

#include "test/support/include/Loader.h"

namespace io::api
{
    auto const ioEtc = []()
    { return support::Loader::getExecutableFolderPath() / "etc" / "io"; };

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
}
