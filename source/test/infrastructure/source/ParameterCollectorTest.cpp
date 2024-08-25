#include <gtest/gtest.h>

#include "lib/infrastructure/include/ParameterCollector.h"
#include "lib/infrastructure/include/ParameterSupplier.h"

namespace infrastructure
{
    class TestSupplier : public ParameterSupplier
    {
        ParameterTypeList list;

    public:
        TestSupplier(ParameterTypeList l) : list(std::move(l))
        {
        }

        ParameterTypeList supplyParameters() const override
        {
            return list;
        }
    };

    TEST(ParameterCollector, empty)
    {
        auto collector = ParameterCollector();
        EXPECT_EQ(collector.getParameters().size(), 0);

        auto supplier = TestSupplier({{"key", "value"}});
        collector.removeParameterSupplier(supplier);
        EXPECT_EQ(collector.getParameters().size(), 0);
        collector.replaceParameterSupplier(supplier, supplier);
        EXPECT_EQ(collector.getParameters().size(), 1);
    }

    TEST(ParameterCollector, add)
    {
        auto collector = ParameterCollector();
        auto supplier = TestSupplier({{"key", "value"}});
        collector.addParameterSupplier(supplier);
        EXPECT_EQ(collector.getParameters().size(), 1);
        EXPECT_EQ(collector.getParameters()[0].first, "key");
        EXPECT_EQ(collector.getParameters()[0].second, "value");
        collector.addParameterSupplier(supplier);
        EXPECT_EQ(collector.getParameters().size(), 2); // The same twice
    }

    TEST(ParameterCollector, remove)
    {
        auto collector = ParameterCollector();
        auto supplier1 = TestSupplier({{"key", "value1"}});
        auto supplier2 = TestSupplier({{"key", "value2"}});
        collector.addParameterSupplier(supplier1).addParameterSupplier(supplier2);
        EXPECT_EQ(collector.getParameters().size(), 2);
        EXPECT_EQ(collector.getParameters()[0].second, "value1");
        EXPECT_EQ(collector.getParameters()[1].second, "value2");
        collector.removeParameterSupplier(supplier1);
        EXPECT_EQ(collector.getParameters().size(), 1);
        EXPECT_EQ(collector.getParameters()[0].second, "value2");
        collector.removeParameterSupplier(supplier2);
        EXPECT_EQ(collector.getParameters().size(), 0);
        collector.removeParameterSupplier(supplier1).removeParameterSupplier(supplier2);
        EXPECT_EQ(collector.getParameters().size(), 0);
    }

    TEST(ParameterCollector, replace)
    {
        auto collector = ParameterCollector();
        auto supplier1 = TestSupplier({{"key", "value1"}});
        auto supplier2 = TestSupplier({{"key", "value2"}});
        auto supplier3 = TestSupplier({{"key", "value3"}});
        auto supplier4 = TestSupplier({{"key", "value4"}});
        collector.addParameterSupplier(supplier1).addParameterSupplier(supplier2).addParameterSupplier(supplier3);
        EXPECT_EQ(collector.getParameters().size(), 3);
        EXPECT_EQ(collector.getParameters()[0].second, "value1");
        EXPECT_EQ(collector.getParameters()[1].second, "value2");
        EXPECT_EQ(collector.getParameters()[2].second, "value3");
        collector.replaceParameterSupplier(supplier2, supplier4);
        EXPECT_EQ(collector.getParameters().size(), 3);
        EXPECT_EQ(collector.getParameters()[0].second, "value1");
        EXPECT_EQ(collector.getParameters()[1].second, "value4");
        EXPECT_EQ(collector.getParameters()[2].second, "value3");
        collector.replaceParameterSupplier(supplier1, supplier2);
        EXPECT_EQ(collector.getParameters().size(), 3);
        EXPECT_EQ(collector.getParameters()[0].second, "value2");
        EXPECT_EQ(collector.getParameters()[1].second, "value4");
        EXPECT_EQ(collector.getParameters()[2].second, "value3");
        collector.replaceParameterSupplier(supplier3, supplier1);
        EXPECT_EQ(collector.getParameters().size(), 3);
        EXPECT_EQ(collector.getParameters()[0].second, "value2");
        EXPECT_EQ(collector.getParameters()[1].second, "value4");
        EXPECT_EQ(collector.getParameters()[2].second, "value1");
        collector.replaceParameterSupplier(supplier3, supplier3);
        EXPECT_EQ(collector.getParameters().size(), 4);
        EXPECT_EQ(collector.getParameters()[0].second, "value2");
        EXPECT_EQ(collector.getParameters()[1].second, "value4");
        EXPECT_EQ(collector.getParameters()[2].second, "value1");
        EXPECT_EQ(collector.getParameters()[3].second, "value3");
    }
}
