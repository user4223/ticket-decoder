// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/interpreter/detail/misc/include/CSVInterpreter.h"
#include "lib/interpreter/api/include/NopSignatureVerifier.h"
#include "lib/interpreter/api/include/NopCertificateProvider.h"

#include "test/support/include/TestSupport.h"

#include "nlohmann/json.hpp"

namespace interpreter::detail::misc
{
    TEST(CSVInterpreter, canInterpret)
    {
        auto interpreter = CSVInterpreter(::test::support::get().getLoggerFactory());
        EXPECT_TRUE(interpreter.canInterpret(common::Context({0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b})));
    }

    TEST(CSVInterpreter, canNotInterpret)
    {
        auto interpreter = CSVInterpreter(::test::support::get().getLoggerFactory());
        EXPECT_FALSE(interpreter.canInterpret(common::Context(std::vector<std::uint8_t>{})));
        EXPECT_FALSE(interpreter.canInterpret(common::Context({0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x19, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b})));
        EXPECT_FALSE(interpreter.canInterpret(common::Context({0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b})));
        EXPECT_FALSE(interpreter.canInterpret(common::Context({0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b})));
    }

    TEST(CSVInterpreter, interpret)
    {
        auto interpreter = CSVInterpreter(::test::support::get().getLoggerFactory());
        auto const data = std::string("02;111111;;00001050;22;190426;200426;1;33333333;4444;555555;180426;8011160;8011113;;6");
        auto context = interpreter.interpret(common::Context({(std::uint8_t *)data.data(), data.size()}));
        // clang-format off
        EXPECT_EQ(nlohmann::json::parse(R"(
            {
                "raw": "02;111111;;00001050;22;190426;200426;1;33333333;4444;555555;180426;8011160;8011113;;6",
                "records": {
                    "CSV": {
                        "classCode": "6",
                        "fromStationIBNR": "8011160",
                        "issuingDate": "2026-04-18",
                        "passengerNumber": 1,
                        "price": 1050,
                        "ticketId": "33333333",
                        "toStationIBNR": "8011113",
                        "validFromDate": "2026-04-19",
                        "validUntilDate": "2026-04-20"
                    }
                },
                "validated": "false"
            })").dump(3), *context.getJson(3));
        // clang-format on
    }

    TEST(CSVInterpreter, interpretInvalidCsv)
    {
        auto interpreter = CSVInterpreter(::test::support::get().getLoggerFactory());
        auto const data = std::string("02;111111;;00001050;22;190426;200426;1;33333333;4444;555555");
        auto context = interpreter.interpret(common::Context({(std::uint8_t *)data.data(), data.size()}));
        // clang-format off
        EXPECT_EQ(nlohmann::json::parse(R"(
            {
                "raw": "02;111111;;00001050;22;190426;200426;1;33333333;4444;555555",
                "validated": "false"
            })").dump(3), *context.getJson(3));
        // clang-format on
    }

    TEST(CSVInterpreter, interpretInvalidAscii)
    {
        auto interpreter = CSVInterpreter(::test::support::get().getLoggerFactory());
        EXPECT_THROW(interpreter.interpret(common::Context({0x20, 0x3b, 0x10})), std::runtime_error);
    }
}
