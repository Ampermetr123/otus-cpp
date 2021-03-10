#include <gtest/gtest.h>
#include <filesystem>
#include <file_dup.h>

namespace fs = std::filesystem;

TEST(TestFileDup, TestFileDup_md5) {
    FileDupFinder ff(4, hash_md5);
    std::list<fs::path> inc_files = {
        fs::path("./test_dirs/dir1/file1_x"),
        fs::path("./test_dirs/dir1/file2_y"),
        fs::path("./test_dirs/dir2/file3_z"),
        fs::path("./test_dirs/dir2/dir2.1/file4_x")
    };
    auto result = ff.find(inc_files);
    ASSERT_EQ(result.size(), 1); // 1 dublicate
    EXPECT_EQ(result.front().size(), 2); // 2 files are the same
    EXPECT_EQ(result.front().front(), fs::path("./test_dirs/dir1/file1_x")); // 2 files are the same
}

TEST(TestFileDup, TestFileDup_sha1) {
    FileDupFinder ff(4, hash_sha1);
    std::list<fs::path> inc_files = {
        fs::path("./test_dirs/dir1/file1_x"),
        fs::path("./test_dirs/dir1/file2_y"),
        fs::path("./test_dirs/dir2/file3_z"),
        fs::path("./test_dirs/dir2/dir2.1/file4_x")
    };
    auto result = ff.find(inc_files);
    ASSERT_EQ(result.size(), 1); // 1 dublicate
    EXPECT_EQ(result.front().size(), 2); // 2 files are the same
    EXPECT_EQ(result.front().front(), fs::path("./test_dirs/dir1/file1_x")); // 2 files are the same
}