#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "commands.h"
#include "storage.h"
#include <vector>


        
class MockStorage : public Storage {
public:
    MOCK_METHOD(storage_result_t, insert, (std::string table, storage_index_t id, storage_data_t data), (override));
    MOCK_METHOD(storage_result_t, truncate, (std::string table), (override));
    MOCK_METHOD(storage_result_t, intersection, (), (override));
    MOCK_METHOD(storage_result_t, difference, (), (override));
};


struct Test_Commands : public ::testing::Test {
    Storage js;
    Test_Commands() : ::testing::Test(), js("A", "B") {};
};


TEST_F(Test_Commands, Test_Command_Unknown) {
    auto [cmd, str] = make_command(js, "ABRACADABRA");
    EXPECT_EQ(cmd, nullptr) << "returned string is " << str;
}


TEST_F(Test_Commands, Test_Command_Insert_Invalid) {
    std::vector<const char*> invalid_insert_commands{
         "INSERT A 42 "
        ,"INSERT A         "
        ,"INSERT A"
        ,"INSERT"
        ,"INSERT A1 -42 Some string"
        ,"insert A 42 Some string"
        ,"INSERT 42 A Some string"
        ,"INSERT X 42 Some string"
    };
    for (const auto inp_str : invalid_insert_commands)  {
        auto [cmd, str] = make_command(js, inp_str);
        EXPECT_EQ(cmd.get(), nullptr) << "with input [" << inp_str << "]";
        EXPECT_FALSE(str.empty()) << "with input [" << inp_str << "]";
    }
}


TEST_F(Test_Commands, Test_Command_Insert_Valid) {
    MockStorage storage;
    const std::string inp_str("INSERT B 42 Some string");
    EXPECT_CALL(storage, insert("B", 42, "Some string")).Times(1);

    auto [cmd, str] = make_command(storage, inp_str);
    
    EXPECT_TRUE(str.empty()) << str;
    ASSERT_NE(cmd, nullptr) << "returned string is " << str;
    auto res = cmd->execute();
}


TEST_F(Test_Commands, Test_Command_Truncate_Invalid) {
    std::vector<const char*> invalid_commands{
         "TRUNCATE A 42 "
        ,"TRUNCATE 42         "
        ,"TRUNCATE X"
        ,"TRUNCATE B19"
        ,"TRUNCATE"
        ,"TRUNCATE   "
        ,"TRUNCATEA"
    };

    for (const auto inp_str : invalid_commands)  {
        auto [cmd, str] = make_command(js, inp_str);
        EXPECT_EQ(cmd.get(), nullptr) << "with input [" << inp_str << "]";
        EXPECT_FALSE(str.empty()) << "with input [" << inp_str << "]";
    }
}


TEST_F(Test_Commands, Test_Command_Truncate_Valid) {
    MockStorage storage;
    const std::string inp_str("TRUNCATE A");
    EXPECT_CALL(storage, truncate("A")).Times(1);

    auto [cmd, str] = make_command(storage, inp_str);
    
    EXPECT_TRUE(str.empty());
    ASSERT_NE(cmd, nullptr);
    auto res = cmd->execute();
}

