// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "lib/api/include/ExecutionPipe.h"

#include "test/support/include/TestSupport.h"

#include <gtest/gtest.h>

namespace api
{

    static auto testPreparator = [](auto const &input)
    { return std::make_optional(ImageBytes{1, 1, 1, std::vector<std::uint8_t>{static_cast<std::uint8_t>(input.bytes.at(0) + 1u)}}); };

    static auto testDetector = [](auto const &input)
    { return std::make_optional(ImageBytes{1, 1, 1, std::vector<std::uint8_t>{static_cast<std::uint8_t>(input.bytes.at(0) + 1u)}}); };

    static auto testDecoder = [](auto const &input)
    { return std::make_optional(BarcodeBytes{std::vector<std::uint8_t>{static_cast<std::uint8_t>(input.bytes.at(0) + 1u)}}); };

    static auto testInterpreter = [](auto const &input)
    { return std::make_optional(Content{std::string(1, input.bytes.at(0) + '0')}); };

    static auto addTo(std::vector<Element> &results)
    {
        return [&results](auto const &input)
        { results.emplace_back(std::move(input)); };
    }

    TEST(ExecutionPipe, fullChain)
    {
        auto &testSupport = ::test::support::get();
        auto results = std::vector<Element>();

        ExecutionPipe(testSupport.getContext())
            .withPreparator(testPreparator)
            .withDetector(testDetector)
            .withDecoder(testDecoder)
            .withInterpreter(testInterpreter)
            .withConsumer(addTo(results))
            .supply(Element(ImageBytes{1, 1, 1, std::vector<std::uint8_t>{0}}, std::string("test")));

        EXPECT_EQ(results.size(), 1);
        EXPECT_TRUE(results.at(0).get());
        EXPECT_EQ(results.at(0).get()->json, "3");
    }

    TEST(ExecutionPipe, minimalChain)
    {
        auto &testSupport = ::test::support::get();
        auto results = std::vector<Element>();

        ExecutionPipe(testSupport.getContext())
            .withDecoder(testDecoder)
            .withInterpreter(testInterpreter)
            .withConsumer(addTo(results))
            .supply(Element(ImageBytes{1, 1, 1, std::vector<std::uint8_t>{0}}, std::string("test")));

        EXPECT_EQ(results.size(), 1);
        EXPECT_TRUE(results.at(0).get());
        EXPECT_EQ(results.at(0).get()->json, "1");
    }
}
