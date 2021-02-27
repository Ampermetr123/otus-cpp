#include <gtest/gtest.h>
#include "generator.h"

TEST(Test_demo, Test_IterGenerator) {
    
    std::vector<int> data=
    { 1,2,3,4,5,6,7,8,9 };
    IterGenerator<std::vector<int>::const_iterator>
        gen(data.cbegin(), data.cend());
    int i = 0;
    for (const auto& item : gen) {
        EXPECT_EQ(item, data[i++]);
    };
    
}