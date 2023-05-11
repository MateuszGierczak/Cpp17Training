#include "catch.hpp"
#include "utils/RangePrinter.hpp"

#include <iostream>
#include <string>

// REFERENCE -> https://en.cppreference.com/w/cpp/language/aggregate_initialization
TEST_CASE("aggregates")
{
    SECTION("c-style array and std::array")
    {
        int arr1[]{1, 2, 3};
        std::array<int, 3> arr2{1, 2, 3};

        std::cout << "c-style array = " << utils::printRange(arr1) << "\n";
        std::cout << "std::array = " << utils::printRange(arr2) << "\n";
    }

    SECTION("brace elision")
    {
        std::array<int, 3> arr1; // WHAT VALUES IS THAT array initialized ?
        std::cout << "arr1 = " << utils::printRange(arr1) << '\n';

        std::array<int, 3> arr2{{1, 2, 3}};
        std::cout << "arr2 = " << utils::printRange(arr2) << '\n';

        struct stretch
        {
            std::array<int, 2> start_point2d{};
            std::array<int, 2> end_point2d{};
        };

        stretch s1{{1, 2}, {3, 3}};
        stretch s2{1, 2, 3, 3};
    }

    SECTION("as a class type")
    {
        SECTION("with no private or protected direct non-static data members")
        {
            struct Person
            {
                std::string full_name{};
            private:
                int age{};
            };

            REQUIRE_FALSE(std::is_aggregate_v<Person>);
        }

        SECTION("c++17 - no user-provided, inherited, or explicit constructors"
                "(explicitly defaulted or deleted constructors are allowed)")
        {
            SECTION("user-provided/explicit constructor")
            {
                struct Person
                {
                    Person() {}

                    std::string full_name{};
                    unsigned age{};
                };

                REQUIRE_FALSE(std::is_aggregate_v<Person>);
            }

            SECTION("explicitly defaulted or deleted constructors are allowed")
            {
                // !!TRAP, fixed in cpp20!!
                struct Person
                {
                    Person() = delete;

                    std::string full_name{};
                    unsigned age{};
                };

                REQUIRE(std::is_aggregate_v<Person>);

                //Person p1; // Doesn't compile in c++17, default constructor is deleted
                Person p2{}; // It compiles in c++17 despite of deleted default constructor
                Person p3 {"Mateusz Gierczak", 25};
            }

        }

        SECTION("c++20 - no user-declared or inherited constructors")
        {
            struct Person
            {
                Person() = default;

                std::string full_name{};
                unsigned age{};
            };

            //REQUIRE_FALSE(std::is_aggregate_v<Person>); // it is true in c++20
        }

        SECTION("no virtual, private or protected base classes")
        {
            struct aggregate_base_class
            {
                std::string s1{};
                std::string s2{};
            };

            struct non_aggregate_base_class
            {
                non_aggregate_base_class(int x) : x_{x} {}

                int x_{};
            };

            // In cpp17 it's aggregate
            struct derived_aggregate : aggregate_base_class, non_aggregate_base_class
            {};

            REQUIRE(std::is_aggregate_v<derived_aggregate>);
        }
    }

    SECTION("initialization")
    {
        constexpr unsigned DEFAULT_SALARY {15000};

        struct Person
        {
            std::string full_name{};
            unsigned age{};
        };

        struct Employee : Person
        {
            // default initialization is allowed for aggregates since c++14
            unsigned salary{DEFAULT_SALARY};
        };

        REQUIRE(std::is_aggregate_v<Person>);
        REQUIRE(std::is_aggregate_v<Employee>);

        SECTION("standard initialization")
        {
            Person p1{"Jan Kowalski", 40};
            Employee e1 {"Jan Kowalski", 40, 20000};
        }

        SECTION("with containers")
        {
            std::vector<Person> persons = {{"Mariusz Kowalski", 25}, {"Jan Kowalski", 40}};
        }

        SECTION("designated initialization")
        {
            Person p1{.full_name{"Jan Kowalski"}, .age{40}};
            //Employee e1 {.full_name{"Jan Kowalski"}, .age{40}, .salary{20000}};
            //unfortunately, it doesn't compile, for explaination please see my question in stack overflow
        }
    }
}
