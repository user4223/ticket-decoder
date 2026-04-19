// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/interpreter/detail/misc/include/CSVInterpreter.h"
#include "lib/interpreter/api/include/NopSignatureVerifier.h"
#include "lib/interpreter/api/include/NopCertificateProvider.h"

#include "test/support/include/TestSupport.h"

namespace interpreter::detail::misc
{
    TEST(CSVInterpreter, Initial)
    {
        auto interpreter = CSVInterpreter(::test::support::get().getLoggerFactory());
        auto const data = std::string("0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15");
        EXPECT_TRUE(interpreter.canInterpret(common::Context({(std::uint8_t const*)data.data(), data.size()})));
    }
}
