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
