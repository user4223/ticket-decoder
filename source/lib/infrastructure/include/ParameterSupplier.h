#pragma once

#include "ParameterCollector.h"

namespace infrastructure
{
    class ParameterSupplier
    {
    public:
        virtual ~ParameterSupplier() = default;

        virtual void supplyParameters(ParameterCollector &collector) const = 0;
    };
}
