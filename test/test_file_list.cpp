#include <gtest/gtest.h>

#include <filesystem>
#include <list>
#include <vector>
#include <file_list.h>

namespace fs = std::filesystem;

TEST(Test_File_list, Test_FileList_recursice) {
    std::list<fs::path> inc_dir = { fs::path("./test_dirs/dir1"), fs::path("./test_dirs/dir2") };
    auto list = build_file_list(inc_dir);
    EXPECT_EQ(list.size(), 4);
}

TEST(Test_File_list, Test_FileList_ecxlude_filter) {
    std::list<fs::path> inc_dir = { fs::path("test_dirs") };
    auto list = build_file_list(inc_dir, { make_filter_exclude_dir(fs::path("test_dirs/dir3")) });
    EXPECT_EQ(list.size(), 4);
}

TEST(Test_File_list, Test_FileList_non_recursive_filter) {
    std::list<fs::path> inc_dir = { fs::path("./test_dirs/dir1"), fs::path("./test_dirs/dir2") };
    auto list = build_file_list(inc_dir, { make_filter_no_subdirs() });
    EXPECT_EQ(list.size(), 3);
}

TEST(Test_File_list, Test_FileList_unicode_1) {
    std::list<fs::path> inc_dir = { fs::path("./test_dirs/dir3") };
    auto list = build_file_list(inc_dir, { make_filter_no_subdirs() });
    EXPECT_EQ(list.size(), 3);
}

TEST(Test_File_list, Test_FileList_unicode_2) {
    std::list<fs::path> inc_dir = { fs::path("./test_dirs/dir3") };
    auto list = build_file_list(inc_dir, { make_filter_wildcard(u8"?ай*.txt") });
    EXPECT_EQ(list.size(), 1);
}

TEST(Test_File_list, Test_FileList_multy_filters) {
    std::list<fs::path> inc_dir = { fs::path("./test_dirs") };
    auto list = build_file_list(inc_dir,
        { make_filter_exclude_dir(fs::path("test_dirs/dir3")),
          make_filter_wildcard(u8"*_x"),
        }
    );
    EXPECT_EQ(list.size(), 2);
}

TEST(Test_File_list, Test_FileList_size_filter) {
    std::list<fs::path> inc_dir = { fs::path("./test_dirs/dir3") };
    auto list = build_file_list(inc_dir,
        { make_filter_exclude_dir(fs::path("test_dirs/dir3")),
          make_filter_min_file_size(4)
        }
    );
    EXPECT_EQ(list.size(), 1);
}