#include <gtest/gtest.h>
#include "iterable_result.h"


TEST(Test_Iterable_Result, Test_ResultString_String) {
    const std::string s = "12345678";
    StringResult r(s);
    const auto& s2 = *r.begin();
    EXPECT_EQ(s, s2);
    EXPECT_TRUE(r.size()==1);
    ASSERT_TRUE(r);
}


TEST(Test_Iterable_Result, Test_ResultString_Iters) {
    const std::string s = "12345678";
    StringResult r(s);
    auto it = r.begin();
    auto it2 = r.begin();
    ++it2;
    auto it_end = r.end();
    ASSERT_TRUE (it_end != it);
}


TEST(Test_Iterable_Result, Test_ResultString_Logic) {
    const std::string s = "12345678";
    StringResult r1(s);
    StringResult r2(s, false);
    EXPECT_TRUE(r1);
    EXPECT_FALSE(r2);
}


TEST(Test_Iterable_Result, Test_ResultString_Empty) {
    const std::string s = "";
    StringResult r(s);
    EXPECT_EQ(*r.begin(), "");
    EXPECT_TRUE(r.size() == 1);
    EXPECT_FALSE( ++r.begin() != r.end());
  
}


TEST(Test_Iterable_Result, Test_SetResult_Access) {
    std::set<std::string> data = { "1", "2", "3"};
    ContResult r(data);
    
    std::string sum;
    for (auto v : r) {
        sum = sum + v;
    };
    
    EXPECT_EQ(r.size(), 3);    
    ASSERT_EQ(sum, "123");
}


TEST(Test_Iterable_Result, Test_ContResult_Empty) {
    std::vector<std::string> v;
    ContResult r(v);
    EXPECT_EQ(r.size(), 0);

}