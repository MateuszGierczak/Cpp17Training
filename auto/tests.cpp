#include "catch.hpp"
#include "range_printer.hpp"

#include <iostream>

void function() {}

TEST_CASE("auto")
{
    auto x = 27;
    const auto const_x = x;
    const auto& const_ref_x = x;

    REQUIRE(std::is_same<decltype(x), int>::value);
    REQUIRE(std::is_same<decltype(const_x), const int>::value);
    REQUIRE(std::is_same<decltype(const_ref_x), const int&>::value);

    SECTION("auto with universal reference")
    {
        auto&& uref1 = x;
        auto&& uref2 = const_x;
        auto&& uref3 = 27;

        //reference collapsing
        REQUIRE(std::is_same<decltype(uref1), int&>::value); //auto&& &
        REQUIRE(std::is_same<decltype(uref2), const int&>::value); //const auto&& &
        REQUIRE(std::is_same<decltype(uref3), int&&>::value); //auto&&
    }

    SECTION("auto with c-style array")
    {
        const int arr[] = {1, 2, 3, 4};

        auto arr1 = arr;
        auto& arr2 = arr;

        REQUIRE(std::is_same<decltype(arr1), const int*>::value);
        REQUIRE(std::is_same<decltype(arr2), const int(&)[4]>::value);
    }

    SECTION("auto with functions")
    {
        auto fun1 = function;
        auto& fun2 = function;

        REQUIRE(std::is_same<decltype(fun1), void(*)()>::value);
        REQUIRE(std::is_same<decltype(fun2), void(&)()>::value);

    }

    SECTION("auto vs. curly braces")
    {
        auto x1 = 27;
        auto x2(27);
        auto x3 = {27};
        auto x4 {27};

        //INTERESTING FACT
        //int x5 {4.5}; //narrowing conversion takes place here but compiler reports error
        int x6(4.5); //narrowing conversion takes place here but compiler doesn't report it

        REQUIRE(std::is_same<int, decltype(x1)>::value);
        REQUIRE(std::is_same<int, decltype(x2)>::value);
        REQUIRE(std::is_same<std::initializer_list<int>, decltype(x3)>::value);
        REQUIRE(std::is_same<int, decltype(x4)>::value);
    }
}

void non_noexcept_function()
{
    std::cout << "non_noexcept_function\n";
}

void noexcept_function() noexcept
{
    std::cout << "noexcept_function\n";
}

TEST_CASE("noexpect as a part of function signature")
{
    REQUIRE_FALSE(std::is_same<decltype(non_noexcept_function), decltype(noexcept_function)>::value); //Before cpp17 type of these two functions was same

    //INTERESTING FACT
    void (*ptr_fun)() = noexcept_function; //non noexcept function pointer binds noexcept function
    ptr_fun();

    ptr_fun = non_noexcept_function;
    ptr_fun();

    //void (*ptr_fun1)() noexcept = non_noexcept_function; //it doesn't work because noexcept function pointer can't bind non noexcept function
}
