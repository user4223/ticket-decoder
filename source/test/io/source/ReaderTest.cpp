// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/input/detail/api/include/Reader.h"

#include "test/support/include/TestSupport.h"

namespace input::detail
{
    auto const ioEtc = []()
    { return ::test::support::get().getIOPath(); };

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
