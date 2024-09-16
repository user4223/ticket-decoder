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
