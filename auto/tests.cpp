#include "catch.hpp"
#include "range_printer.hpp"

struct Class
{
    void const_function() const {}
    void noexcept_function() noexcept {}
};

TEST_CASE("auto type deduction")
{
    auto x = 27;
    const auto const_x = 27;
    auto& ref_x = x;
    auto& const_ref_x = const_x;

    REQUIRE(std::is_same_v<decltype(x), int>);
    REQUIRE(std::is_same_v<decltype(const_x), const int>);
    REQUIRE(std::is_same_v<decltype(ref_x), int&>);
    REQUIRE(std::is_same_v<decltype(const_ref_x), const int&>);

    SECTION("with c-style arrays")
    {
        constexpr std::size_t ARRAY_SIZE = 10;
        int arr[ARRAY_SIZE] = {};

        auto val_arr = arr;
        auto& ref_arr = arr;

        REQUIRE(std::is_same_v<decltype(val_arr), int*>);
        REQUIRE(std::is_same_v<decltype(ref_arr), int(&)[ARRAY_SIZE]>);
    }

    SECTION("with functions")
    {
        auto fun1 = &Class::noexcept_function;
        REQUIRE(std::is_same_v<decltype(fun1), void (Class::*)() noexcept>); //noexcept is a part of function signature since c++17

        auto fun2 = &Class::const_function;
        REQUIRE(std::is_same_v<decltype(fun2), void(Class::*)() const>);
    }

    SECTION("with curly-bracket")
    {
        auto x1 = 27;
        auto x2(27);
        auto x3 = { 27 };
        auto x4{ 27 };

        // INTERESTING FACT
        // int x5 {4.5}; //narrowing conversion takes place here but compiler reports error
        int x6(4.5); // narrowing conversion takes place here but compiler doesn't report it

        REQUIRE(std::is_same_v<decltype(x1), int>);
        REQUIRE(std::is_same_v<decltype(x2), int>);
        REQUIRE(std::is_same_v<decltype(x3), std::initializer_list<int>>);
        REQUIRE(std::is_same_v<decltype(x4), int>);
    }
}
