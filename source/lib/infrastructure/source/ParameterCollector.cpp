#include "../include/ParameterCollector.h"

#include <numeric>

namespace infrastructure
{
    ParameterCollector &ParameterCollector::addParameterSupplier(ParameterSupplier const &supplier)
    {
        suppliers.emplace_back(&supplier);
        return *this;
    }

    ParameterSupplier::ParameterTypeList ParameterCollector::getParameters() const
    {
        return std::reduce(std::begin(suppliers), std::end(suppliers), ParameterSupplier::ParameterTypeList{}, [](auto &&list, auto const *item)
                           {
                            auto parameters = item->supplyParameters();
                            std::for_each(std::begin(parameters), std::end(parameters), [&](auto &&parameter)
                            {  list.emplace_back(std::move(parameter)); });
                            return list; });
    }
}
