#pragma once

#include <map>
#include <string>

namespace infrastructure
{
    class ParameterCollector
    {
    public:
        using ParameterType = std::pair<std::string, std::string>;

        virtual ~ParameterCollector() = default;

        virtual ParameterCollector &addParameter(ParameterType parameterType) = 0;
    };
}
