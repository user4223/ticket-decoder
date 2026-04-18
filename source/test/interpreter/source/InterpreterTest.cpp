// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/interpreter/api/include/Interpreter.h"
#include "lib/interpreter/api/include/NopSignatureVerifier.h"
#include "lib/interpreter/api/include/NopCertificateProvider.h"

#include "test/support/include/TestSupport.h"

namespace interpreter::api
{

    TEST(Interpreter, UnknownDataGetsIgnored)
    {
        auto &context = ::test::support::get().getContext();
        auto const signatureVerifier = NopSignatureVerifier::create(context);
        auto const certificateProvider = NopCertificateProvider::create(context);
        auto interpreter = Interpreter::create(context, *signatureVerifier, *certificateProvider);

        EXPECT_NO_THROW(EXPECT_FALSE(interpreter->interpret({0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1}, "nirvana").has_value()));
    }
}
