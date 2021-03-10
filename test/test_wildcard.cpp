#include <gtest/gtest.h>
#include <wildcard.h>
#include <clocale>
#include <string>
#include <regex>

TEST(Test_Wildcard, Test_Wildcard) {
    EXPECT_TRUE(Wildcard("*").match("/dir/file"));
    EXPECT_TRUE(Wildcard("some?file").match("someXfile"));
    EXPECT_TRUE(Wildcard("some*").match("something.txt"));
    EXPECT_TRUE(Wildcard(".*").match(".some"));
    EXPECT_TRUE(Wildcard("file").match("file"));
    EXPECT_TRUE(Wildcard("*middle*.tx?").match("file_middle___.txt"));
    EXPECT_TRUE(Wildcard("???.???").match("123.123"));
    EXPECT_TRUE(Wildcard("?il*.txt").match("file.txt"));
    EXPECT_TRUE(Wildcard("?i*e.txt").match("file.txt"));
    
    EXPECT_FALSE(Wildcard("file").match("Xfile"));
    EXPECT_FALSE(Wildcard("?ile").match("ffile"));
    EXPECT_FALSE(Wildcard("1*3").match("1234"));
    EXPECT_FALSE(Wildcard("*3*").match("rrrrr"));
}

TEST(Test_Wildcard, Test_Wildcard_Multi) {
    Wildcard w(std::vector<std::string>{"triangle", "s?uare", "ci*e"});
    EXPECT_TRUE(w.match("triangle"));
    EXPECT_TRUE(w.match("square"));
    EXPECT_TRUE(w.match("circle"));
    
    EXPECT_FALSE(w.match("megagatriangle"));
    EXPECT_FALSE(w.match("tristan"));
    EXPECT_FALSE(w.match("triangle|square"));
    EXPECT_FALSE(w.match("circle."));
}

TEST(Test_Wildcard, Test_Wildcard_to_wide_string) {
    EXPECT_EQ(std::wstring(L"Magenta"), wildcard_impl::to_wide_string(std::u8string(u8"Magenta")));
    EXPECT_EQ(std::wstring(L"Чёрный"), wildcard_impl::to_wide_string(std::u8string(u8"Чёрный")));
}

TEST(Test_Wildcard, Test_Wildcard_Unicode) {
    EXPECT_TRUE(Wildcard(u8"?ай*л.txt").match(u8"файл.txt"));
    EXPECT_TRUE(Wildcard(u8"?ай*").match(u8"файл.txt"));
    EXPECT_TRUE(Wildcard(u8"?ай*л.txt").match(u8"файл.txt"));
    EXPECT_TRUE(Wildcard(u8"?il*.txt").match(u8"file.txt"));
    EXPECT_TRUE(Wildcard(u8"*").match(u8"файл"));
    EXPECT_TRUE(Wildcard(u8"ф*").match(u8"файл"));
    EXPECT_TRUE(Wildcard(u8"?айл").match(u8"файл"));
    EXPECT_TRUE(Wildcard(u8"*.h").match(u8"metafile.h"));
}

TEST(Test_Wildcard, Test_wregex) {
    std::locale::global(std::locale(""));
    using namespace std;
    wstring re(L".ай.*л\\.txt");
    wstring m(L"файл.txt");
    wregex reg(re);
    EXPECT_TRUE(regex_match(m,reg));
}