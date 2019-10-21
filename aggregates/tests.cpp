#include "catch.hpp"
#include "range_printer.hpp"

#include <iostream>
#include <string>

struct SimpleAggregate
{
    std::string str {};
    std::array<int, 3> arr;
    int i;
};

TEST_CASE("aggregates")
{
    SECTION("Simple aggregate")
    {
        SimpleAggregate a {"test", {1, 2, 3}, 4};
    }
}
