#include "catch.hpp"
#include "range_printer.hpp"

#include <iostream>
#include <string>

using namespace std::string_literals;

template <typename T>
void hash_combine(std::size_t& seed, const T& arg)
{
    seed += std::hash<T>{}(arg) + 2828293 + (seed >> 5) + (seed << 2);
}

template <typename T, typename... Ts>
std::size_t hash(const T& arg, const Ts&... args)
{
    std::size_t seed{ 0 };
    (hash_combine(seed, arg), ..., hash_combine(seed, args));
    return seed;
}

TEST_CASE("hash - write a function that calculate hash value for arguments")
{
    REQUIRE(hash("THX"s, 1011, 105.4) == 15032652827347851566);
    REQUIRE(hash("THZ"s, "TXC"s) == 17967872512761610900);
    REQUIRE(hash(10) == 2828303);
}

struct Person
{
    Person(std::string _name, std::string _surname, unsigned _age)
        : name(_name), surname(_surname), age(_age)
    {}

    auto tie() const
    {
        return std::tie(name, surname, age);
    }

private:
    std::string name;
    std::string surname;
    unsigned age;
};

template <typename... Ts, std::size_t... Is>
std::size_t hash_tuple_impl(const std::tuple<Ts...>& t, std::index_sequence<Is...>)
{
    return hash(std::get<Is>(t)...);
}

template <typename... Ts>
std::size_t hash_tuple(const std::tuple<Ts...>& t)
{
    return hash_tuple_impl(t, std::index_sequence_for<Ts...>{});
}

TEST_CASE("hash_combine - write a function that calculate hash for tuple")
{
    Person p{ "Mariusz", "Kowalski", 25 };

    REQUIRE(hash_tuple(p.tie()) == 3834454928984662410);
    REQUIRE(hash_tuple(std::make_tuple(10.4, 5, "THX"s)) == 11840747206787424437);
}

template<unsigned ID, unsigned... IDs>
constexpr bool has_id(unsigned id)
{
    return ((id == ID) || ... || (id == IDs));
}

TEST_CASE("has_is - write a function that checks if ID is in parameter pack")
{
    REQUIRE(has_id<100, 200, 300>(100));
    REQUIRE_FALSE(has_id<100, 200, 300>(400));
    REQUIRE(has_id<100>(100));
}

template<typename Container, typename Arg, typename... Args>
std::size_t matches(const Container& c, Arg arg, Args... args)
{
    return (std::count(std::begin(c), std::end(c), arg) + ... + std::count(std::begin(c), std::end(c), args));
}

TEST_CASE("matches - write a function that calculates number of elements equal to arguments")
{
    std::vector<int> v {1, 2, 3, 4, 5};

    REQUIRE(matches(v, 2, 3) == 2);
    REQUIRE(matches(v, 6, 7, 8) == 0);
}
