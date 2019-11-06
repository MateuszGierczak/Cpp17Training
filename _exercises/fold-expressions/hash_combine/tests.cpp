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

TEST_CASE("hash - write a function that calculate hash for tuple")
{
    Person p{ "Mariusz", "Kowalski", 25 };

    REQUIRE(hash_tuple(p.tie()) == 3834454928984662410);
    REQUIRE(hash_tuple(std::make_tuple(10.4, 5, "THX"s)) == 11840747206787424437);
}
