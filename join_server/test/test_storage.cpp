#include <gtest/gtest.h>
#include "iterable_result.h"
#include "storage.h"


struct Test_Storage : public ::testing::Test {
    Storage js;
    std::string intersect_res;
    std::string difference_res;
    std::string table_a_only_res;
    Test_Storage() : ::testing::Test(), js("A", "B") {};

    void SetUp() override {
        js.insert("A", 0, "lean");
        js.insert("A", 1, "sweater");
        js.insert("A", 2, "frank");
        js.insert("A", 3, "violation");
        js.insert("A", 4, "quality");
        js.insert("A", 5, "precision");
        
        js.insert("B", 3, "proposal");
        js.insert("B", 4, "example");
        js.insert("B", 5, "lake");
        js.insert("B", 6, "flour");
        js.insert("B", 7, "wonder");
        js.insert("B", 8, "selection");

        table_a_only_res = "0,lean,"
            "1,sweater,"
            "2,frank,"
            "3,violation,"
            "4,quality,"
            "5,precision,";


        intersect_res = "3,violation,proposal"
            "4,quality,example"
            "5,precision,lake";

        difference_res = "0,lean,"
            "1,sweater,"
            "2,frank,"
            "6,,flour"
            "7,,wonder"
            "8,,selection";
    }
};


TEST_F(Test_Storage, Test_Intersection) {
    auto res = js.intersection();
    std::string sum;
    for (auto s : *res) {
        sum.append(s);
    };
    ASSERT_EQ(sum, intersect_res);
}


TEST_F(Test_Storage, Test_Difference) {
    auto res = js.difference();
    std::string sum;
    for (auto s : *res) {
        sum.append(s);
    };
    ASSERT_EQ(sum, difference_res);
}

TEST_F(Test_Storage, Test_Truncate_then_Difference) {
    auto r1 = js.truncate("B");
    ASSERT_TRUE(*r1);
    auto res = js.difference();
    std::string sum;
    for (auto s : *res) {
        sum.append(s);
    };
    ASSERT_EQ(sum, table_a_only_res);
}