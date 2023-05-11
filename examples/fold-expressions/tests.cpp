#include "catch.hpp"

#include <iostream>

namespace cpp11
{
    auto sum()
    {
        return 0;
    }

    template <typename T, typename... Ts>
    auto sum(T t, Ts... ts)
    {
        return t + sum(ts...);
    }
}

// C++ Weekly - Ep 20 C++17's Fold Expressions - Introduction
// Trick with initializer list explained in link below
//                          |||
//                          vvv
// https://articles.emptycrate.com/2016/05/14/folds_in_cpp11_ish.html
namespace cpp14
{
    template <typename... Ts>
    auto sum(Ts... ts)
    {
        typename std::common_type<Ts...>::type result{};
        (void)std::initializer_list<int>{ (result += ts, 0)... };
        return result;
    }
}

namespace cpp17
{
    template <typename... Ts>
    auto sum(Ts... ts)
    {
        return (ts + ...);
    }

    // FIRST VERSION
    /*template <typename... Ts>
    auto avg(Ts... ts)
    {
        return (... + ts) / sizeof...(ts);
    }*/

    template <typename T, typename... Ts>
    auto avg(T t, Ts... ts)
    {
        return (t + ... + ts) / (sizeof...(ts) + 1);
    }

    template <typename... Ts>
    void print(Ts&&... ts)
    {
        ((std::cout << std::forward<Ts>(ts) << " "), ...);
        std::cout << "\n";
    }
}

TEST_CASE("Fold expresion")
{
    auto sum_cpp11 = cpp11::sum(1, 2.0, 3.5);
    auto sum_cpp14 = cpp14::sum(1, 2.0, 3.5);
    auto sum_cpp17 = cpp17::sum(1, 2.0, 3.5);

    REQUIRE(sum_cpp11 == sum_cpp14);
    REQUIRE(sum_cpp11 == sum_cpp17);

    auto avg_cpp17 = cpp17::avg(1, 1.5, 5.0);
    REQUIRE(avg_cpp17 == 2.5);
    auto avg2_cpp17 = cpp17::avg(1);

    std::string thx("THX");
    cpp17::print(10, 150.4, thx);
}
