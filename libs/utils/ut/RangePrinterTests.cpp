#include <gtest/gtest.h>
#include "utils/RangePrinter.hpp"

using namespace ::testing;

template <typename Range>
std::string toString(const Range& range)
{
    std::stringstream os;
    os << utils::printRange(range);
    return os.str();
}

TEST(RangePrinterTests, shouldPrintVector)
{
    std::vector<int> empty_vec{};
    EXPECT_EQ(toString(empty_vec), "[]");

    std::vector<int> vec_int{1, 2};
    EXPECT_EQ(toString(vec_int), "[1, 2]");

    std::vector<char> vec_char{'a', 'b', 'c'};
    EXPECT_EQ(toString(vec_char), "[a, b, c]");
    
    std::vector<std::string> vec_str{"Simple", "Test", "Case"};
    EXPECT_EQ(toString(vec_str), "[Simple, Test, Case]");
}

TEST(RangePrinterTests, shouldPrintInnerContainer)
{
    std::vector<std::set<int>> vec_with_set {{1, 2, 3}, {4, 5, 6}};
    EXPECT_EQ(toString(vec_with_set), "[[1, 2, 3], [4, 5, 6]]");

    std::vector<std::vector<std::string>> vec_with_str_vec {{"Test", "Suite"}};
    EXPECT_EQ(toString(vec_with_str_vec), "[[Test, Suite]]");

    std::map<int, std::vector<std::string>> map_with_vec{{1, {"Test", "Suite"}}};
    EXPECT_EQ(toString(map_with_vec), "[{1, [Test, Suite]}]");
}

TEST(RangePrinterTests, shouldPrintMap)
{
    std::map<int, int> map_int{{1, 3}, {2, 4}};
    EXPECT_EQ(toString(map_int), "[{1, 3}, {2, 4}]");

    std::map<int, std::string> map_string{{1, "Test"}, {2, "Suite"}};
    EXPECT_EQ(toString(map_string), "[{1, Test}, {2, Suite}]");
}
