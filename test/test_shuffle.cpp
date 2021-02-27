#include <gtest/gtest.h>
#include "shuffle.h"
#include <list>
#include <algorithm>
#include <functional>

TEST(Test_Shuffle, Test_Shuffle_Equal_Parts) {
   
    // 4 x 8
    std::vector<std::vector<int>> data_in = {
     { 1,2,3,3,4,4,7,8 },
     { 1,2,3,3,4,4,7,8 },
     { 1,2,3,3,4,4,7,8 },
     { 1,2,3,3,4,4,7,8 }
    };
    std::vector<std::vector<int>> data_out(2);

    shuffle(data_in, data_out);

    // Equal Parts
    EXPECT_EQ(data_out[0].size(), 16); 
    EXPECT_EQ(data_out[1].size(), 16);


}

TEST(Test_Shuffle, Test_Shuffle_Unique) {
    // 4 x 6
    std::vector<std::vector<int>> data_in = {
     { 1,1,3,3,5,5 },
     { 1,1,3,3,5,5 },
     { 1,1,3,3,5,5 },
     { 1,1,3,3,5,5 }
    };
    std::vector<std::vector<int>> data_out(2);

    shuffle(data_in, data_out);
    std::vector<int> intersect;
    std::set_intersection(data_out[0].begin(), data_out[0].end(),
                          data_out[1].begin(), data_out[1].end(),
                            std::back_inserter(intersect),
                            std::greater<int>());
    EXPECT_EQ(intersect.size(), 0);


    std::vector<int> v0, v1;
    std::unique_copy(data_out[0].begin(), data_out[0].end(), std::back_inserter(v0));
    std::unique_copy(data_out[1].begin(), data_out[1].end(), std::back_inserter(v1));
    EXPECT_EQ(v0.size(), 2);
    EXPECT_EQ(v1.size(), 1);

}
