// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/ParameterCollector.h"

#include <numeric>
#include <algorithm>

namespace infrastructure
{
    ParameterCollector &ParameterCollector::addParameterSupplier(ParameterSupplier const &supplier)
    {
        suppliers.emplace_back(&supplier);
        return *this;
    }

    ParameterCollector &ParameterCollector::removeParameterSupplier(ParameterSupplier const &supplier)
    {
        std::erase_if(suppliers, [&](auto const *item)
                      { return &supplier == item; });
        return *this;
    }

    ParameterCollector &ParameterCollector::replaceParameterSupplier(ParameterSupplier const &supplier, ParameterSupplier const &replacement)
    {
        auto const match = std::find_if(std::begin(suppliers), std::end(suppliers), [&](auto const *item)
                                        { return &supplier == item; });
        if (match == suppliers.end())
        {
            addParameterSupplier(replacement);
            return *this;
        }
        std::replace(match, match + 1, &supplier, &replacement);
        return *this;
    }

    ParameterSupplier::ParameterTypeList ParameterCollector::getParameters() const
    {
        return std::accumulate(std::begin(suppliers), std::end(suppliers), ParameterSupplier::ParameterTypeList{}, [](auto &&list, auto const *item)
                               {
                            auto parameters = item->supplyParameters();
                            std::for_each(std::begin(parameters), std::end(parameters), [&](auto &&parameter)
                            {  list.emplace_back(std::move(parameter)); });
                            return std::move(list); });
    }
}
