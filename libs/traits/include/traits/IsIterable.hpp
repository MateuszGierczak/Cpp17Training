#pragma once

namespace traits
{
template<typename, typename = void>
constexpr bool is_iterable{};

template<typename T>
constexpr bool is_iterable<
    T,
    std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>
> = true;
}