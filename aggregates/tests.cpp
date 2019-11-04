#include "catch.hpp"
#include "range_printer.hpp"

#include <iostream>
#include <string>

/* PRESENTATION PLAN
 * 1. C-style and std::array as agregate
 * 2. Interesting fact about std::array - it has no a constructor
 * 3. Brace elision
 * 4. Custom class/struct as aggregate
 * 5. Default initialization in class/struct
 * 6. Custom aggregates with containers
 * 7. New restriction about user-declared or inherited constructors in cpp20
 * 8. Non aggregate as a base class/struct of aggregate in cpp17
 * 9. New way of aggregate initialization in cpp20
 */

constexpr unsigned DEFAULT_AGE = 100;
constexpr unsigned DEFAULT_SALARY = 10000;

struct Person
{
    // In cpp20 user-declared or inherited constructors are not allowed for aggregates
    // Person() = default;

    std::string name_{};
    std::string surname_{};
    unsigned age_{ DEFAULT_AGE };
};

struct Employee : Person
{
    unsigned salary_{ DEFAULT_SALARY };
};

struct aggregate_base_class
{
    std::string s1{};
    std::string s2{};
};

struct non_aggregate_base_class
{
    non_aggregate_base_class(int x) : x_{ x }
    {}

    int x_{};
};

// In cpp17 it's aggregate
struct derived_aggregate : aggregate_base_class, non_aggregate_base_class
{};

struct stretch
{
    std::array<int, 2> start_point2d{};
    std::array<int, 2> end_point2d{};
};

// REFERENCE -> https://en.cppreference.com/w/cpp/language/aggregate_initialization
TEST_CASE("aggregates")
{
    SECTION("c-style arrays")
    {
        int arr1[] = { 1, 2, 3 };
        auto [x, y, z] = arr1;
        REQUIRE(x == 1);
        REQUIRE(y == 2);
        REQUIRE(z == 3);
    }

    SECTION("std::array")
    {
        std::array<int, 3> arr1{ 1, 2, 3 };
        auto [x, y, z] = arr1;
        REQUIRE(x == 1);
        REQUIRE(y == 2);
        REQUIRE(z == 3);

        // REFERENCE -> https://en.cppreference.com/w/cpp/container/array
        std::array<int, 3> arr2; // WHAT VALUES IS THAT array initialized ?
        std::cout << "arr2 = " << utils::print_range(arr2) << "\n";

        std::array<int, 3> arr3 = { { 1, 2, 3 } }; // INTERESTING FACT - why it is possible ?
                                                   // BRACE ELISION
        REQUIRE(std::equal(std::begin(arr1), std::end(arr1), std::begin(arr3), std::end(arr3)));
    }

    SECTION("brace elision")
    {
        stretch s1{ { 1, 2 }, { 3, 3 } };
        stretch s2{ 1, 2, 3, 3 };
    }

    SECTION("as a struct or class")
    {
        REQUIRE(std::is_aggregate_v<Person>);

        Person p1{ "Marek", "Kowalski", 24 };
        auto& [name, surname, age] = p1;
        REQUIRE(p1.age_ == 24);

        Person p2{ "Jan", "Kowalski" };
        REQUIRE(p2.age_ == DEFAULT_AGE);

        SECTION("with containers")
        {
            std::vector<Employee> employees = { { "Mateusz", "Kowalski", 24, 15000 },
                                                { "Mariusz", "Kowalkiewicz", 26 } };
        }

        SECTION("inherited from non aggregate class")
        {
            REQUIRE(std::is_aggregate_v<aggregate_base_class>);
            REQUIRE_FALSE(std::is_aggregate_v<non_aggregate_base_class>);
            REQUIRE(std::is_aggregate_v<derived_aggregate>);
        }

        SECTION("cpp20")
        {
            Person p3{ .name_{ "Mateusz" }, .surname_{ "Michalek" }, .age_{ 100 } };
            REQUIRE(p3.age_ == 100);

            Person p4{ .name_{ "Mariusz" } };
            REQUIRE(p4.surname_ == "");
            REQUIRE(p4.age_ == DEFAULT_AGE);
        }
    }
}
