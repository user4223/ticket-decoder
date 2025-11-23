// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <map>
#include <string>
#include <vector>

namespace infrastructure
{
    class ParameterSupplier
    {
    public:
        using ParameterType = std::pair<std::string, std::string>;
        using ParameterTypeList = std::vector<ParameterType>;

        virtual ~ParameterSupplier() = default;

        virtual ParameterTypeList supplyParameters() const = 0;
    };
}
