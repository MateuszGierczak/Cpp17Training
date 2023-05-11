#include "catch.hpp"

#include <iostream>

template<typename T, std::size_t N>
struct array
{
    T data[N];
};

template<typename T, typename... Ts>
array(T, Ts...) -> array<std::enable_if_t<(std::is_same_v<T, Ts> && ...), T>,
                         1 + sizeof...(Ts)>;

TEST_CASE("Write deduction guide for array type")
{
    array a1{10, 20, 30, 40};
    REQUIRE(std::size(a1.data) == 4);

    //array a2 {10.0, 30}; // it should not compile
}
