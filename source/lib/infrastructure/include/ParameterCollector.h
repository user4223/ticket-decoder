#pragma once

#include "ParameterSupplier.h"

namespace infrastructure
{
    class ParameterCollector
    {
        std::vector<ParameterSupplier const *> suppliers;

    public:
        ParameterCollector &addParameterSupplier(ParameterSupplier const &supplier);

        ParameterSupplier::ParameterTypeList getParameters() const;
    };
}
