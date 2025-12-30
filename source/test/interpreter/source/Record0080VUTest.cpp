// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef WITH_UIC_INTERPRETER

#include <gtest/gtest.h>

#include "lib/interpreter/detail/uic918/include/Record0080VU.h"

namespace interpreter::detail::uic
{
    TEST(Record0080VU, getProduktbezeichnung)
    {
        EXPECT_EQ("City-Ticket", *Record0080VU::getProduktbezeichnung(6262, 2000));
        EXPECT_EQ("Deutschlandticket", *Record0080VU::getProduktbezeichnung(6263, 9999));
        EXPECT_FALSE(Record0080VU::getProduktbezeichnung(6262, 42).has_value());
        EXPECT_FALSE(Record0080VU::getProduktbezeichnung(23, 2000).has_value());
        EXPECT_FALSE(Record0080VU::getProduktbezeichnung(23, 42).has_value());
    }
}

#endif
