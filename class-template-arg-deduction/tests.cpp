#include "catch.hpp"
#include "range_printer.hpp"

#include <boost/type_index.hpp>

#include <iostream>
#include <map>
#include <string>
#include <tuple>

using namespace boost::typeindex;

template <typename T>
void value_arg_function(T x)
{
    std::cout << __FUNCTION__ << ": " << type_id_with_cvr<decltype(x)>().pretty_name() << "\n";
}

template <typename T>
void ref_arg_function(T& x)
{
    std::cout << __FUNCTION__ << ": " << type_id_with_cvr<decltype(x)>().pretty_name() << "\n";
}

template <typename T, std::size_t N>
constexpr std::size_t array_size(const T (&)[N])
{
    return N;
}

template <typename T>
void uref_arg_function(T&& x)
{
    std::cout << __FUNCTION__ << ": " << type_id_with_cvr<decltype(x)>().pretty_name() << "\n";
}

TEST_CASE("Template argument deduction")
{
    int x = 27;
    const int const_x = 27;
    int& ref_x = x;
    const int& const_ref_x = const_x;

    constexpr std::size_t ARRAY_SIZE = 10;
    int arr[ARRAY_SIZE] = {};

    SECTION("function argument as value")
    {
        value_arg_function(x); // T = int
        value_arg_function(const_x); // T = int, constness is ignored
        value_arg_function(ref_x); // T = int, referencensess is also ignored
        value_arg_function(const_ref_x); // T = int, constness and referencness is ignored
        value_arg_function(arr); // T = int*, c-style array is pointer to first element
    }

    SECTION("function argument as ref")
    {
        ref_arg_function(x);
        ref_arg_function(const_x);
        ref_arg_function(ref_x);
        ref_arg_function(const_ref_x);
        ref_arg_function(arr);

        static_assert(array_size(arr) == ARRAY_SIZE);
    }

    SECTION("function argument as universal ref")
    {
        uref_arg_function(x); // T = int&, reference collapsing T&& & = T&

        int&& rref_x = 27;
        uref_arg_function(std::move(rref_x)); // T = int&&, reference collapsing T&& && = T&&

        uref_arg_function(27); // T = int&&, same rule as above
    }
}

namespace training
{
    template <typename T, typename U>
    struct pair
    {
        T first_{};
        U second_{};

        pair(const T& first, const T& second) : first_{ first }, second_{ second }
        {}

        pair(T&& first, U&& second) : first_{ std::move(first) }, second_{ std::move(second) }
        {}
    };
}

TEST_CASE("Class template argument deduction - how it works")
{
    using namespace std::string_literals;

    training::pair p1{ "THX"s, 694 };
    REQUIRE(std::is_same_v<decltype(p1), training::pair<std::string, int>>);

    // HOW IT WORKS
    // REFERENCE -> Class Template Argument Deduction in C++17 - Timur Doumler [ACCU 2018]
    // 1. Check if class is template class
    // 2. Treat constructors as functions
    // 3. Submit template type deduction and overload resolution to choose function
    // 4. Deduce types for class
    // 5. Instanstiate it
}

TEST_CASE("Class template argument deduction")
{
    SECTION("with std::pair")
    {
        // In cpp98
        std::pair<std::string, int> p1{ "THX", 1011 };

        // In cpp11
        auto p2 = std::make_pair("THX", 1011); // p2 = std::pair<const char*, int>

        // In cpp14
        using namespace std::string_literals;

        auto p3 = std::make_pair("THX"s, 1011);

        // In cpp17
        std::pair p4{ "THX"s, 1011 };
        REQUIRE(std::is_same_v<decltype(p4), std::pair<std::string, int>>);
    }

    SECTION("with std::tuple")
    {
        using namespace std::string_literals;

        std::tuple<std::string, int, double> t1{ "THX", 1011, 10.5 };
        std::tuple t2{ "THX"s, 1011, 10.5 };

        REQUIRE(std::is_same_v<decltype(t1), decltype(t2)>);
    }

    SECTION("class vs template type deduction - rule all or nothing")
    {
        // std::tuple<std::string> t1{ "THX", 1011, 10.5 }; - ERROR: partially specified class
        // template argument

        auto t2 = std::make_tuple<std::string>("THX", 1011, 10.5); // for make_tuple it is possible
    }

    SECTION("with std::vector")
    {
        std::vector<int> v1{ 3 };
        std::cout << "v1 = " << utils::print_range(v1) << "\n";

        std::vector<int> v2(3);
        std::cout << "v2 = " << utils::print_range(v2) << "\n";

        std::vector v3{ 3 };
        // std::vector v4(3); - class template argument deduction dosnt work here

        std::vector range{ 1, 2, 3, 4, 5, 10 };
        std::vector copy_range1(std::begin(range), std::end(range)); // but how it works ?
        std::vector copy_range2{ std::begin(range), std::end(range) };

        REQUIRE(range == copy_range1);
        std::cout << type_id_with_cvr<decltype(copy_range1)>().pretty_name() << "\n";
        std::cout << type_id_with_cvr<decltype(copy_range2)>().pretty_name() << "\n";
    }

    SECTION("with std::map - no CTAD in nested types")
    {
        using namespace std::string_literals;

        // std::map m1{ { "THX"s, 1011 }, { "THZ"s, 1012 } }; - it doesn't work :(
        // std::vector<std::pair> v1{ { "THX"s, 1011 }, { "THZ"s, 1012 } };
        std::map<std::string, int> m2{ { "THX"s, 1011 }, { "THZ"s, 1012 } };
    }
}

namespace training
{
    template <typename T>
    struct vector
    {
        template <typename Iterator>
        vector(Iterator begin, Iterator end)
        {
            /*...*/
        }
    };

    template <typename Iterator>
    vector(Iterator, Iterator)->vector<typename std::iterator_traits<Iterator>::value_type>;
}

TEST_CASE("Deduction guide")
{
    std::vector range{ 1, 2, 3, 4, 10 };

    training::vector copy_range1(std::begin(range), std::end(range));
    std::cout << type_id_with_cvr<decltype(copy_range1)>().pretty_name() << "\n";

    REQUIRE(std::is_same_v<decltype(copy_range1), training::vector<int>>);

    SECTION("list-initialisation has priority")
    {
        std::vector v1(std::begin(range), std::end(range));
        REQUIRE(std::is_same_v<decltype(v1), std::vector<int>>);

        std::vector v2{ std::begin(range), std::end(range) };
        REQUIRE_FALSE(std::is_same_v<decltype(v2), std::vector<int>>);
    }
}
