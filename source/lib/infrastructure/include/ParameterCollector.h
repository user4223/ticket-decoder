// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ParameterSupplier.h"

#include <vector>

namespace infrastructure
{
    class ParameterCollector
    {
        std::vector<ParameterSupplier const *> suppliers;

    public:
        ParameterCollector &addParameterSupplier(ParameterSupplier const &supplier);

        ParameterCollector &removeParameterSupplier(ParameterSupplier const &supplier);

        ParameterCollector &replaceParameterSupplier(ParameterSupplier const &supplier, ParameterSupplier const &replacement);

        ParameterSupplier::ParameterTypeList getParameters() const;
    };
}
