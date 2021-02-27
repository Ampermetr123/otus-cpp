#include <gtest/gtest.h>
#include <list>
#include <algorithm>
#include <functional>

#include <helpers.h>

TEST(Test_Helpers, Test_max_size_of_containers) {
    using namespace std;
   vector<vector<int>> v1 = {vector<int>(3), vector<int>(42), vector<int>(0)};
   auto max1 = max_size_of_containers(v1);
   EXPECT_EQ(max1, 42);

   vector<vector<int>> v2 = {};
   auto max2 = max_size_of_containers(v2);
   EXPECT_EQ(max2, 0);
}



TEST(Test_Helpers, maxval_of_ordered_conts) {
   using namespace std;
   vector<vector<int>> v1 = { {0, 0, 0}, {}, {-1, 19, 42} };
   auto max1 = maxval_of_ordered_conts(v1);
   EXPECT_EQ(max1, 42);


   vector<vector<int>> v2 = { {}, {}, {} };
   auto max2 = maxval_of_ordered_conts(v2);
   EXPECT_EQ(max2, 0);
}



TEST(Test_Helpers, Test_less_filled_cont) {
   using namespace std;
   vector<vector<int>> v1 = { {0, 0, 0}, {3}, {-1, 19, 42} };
   auto p1 = less_filled_cont(v1);
   EXPECT_EQ(p1, &v1[1]);


   vector<vector<int>> v2 = { {0}, {0}, {} };
   auto p2 = less_filled_cont(v2);
   EXPECT_EQ(p2, &v2[2]);
}
