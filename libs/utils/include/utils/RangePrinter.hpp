#pragma once

#include <ostream>
#include "traits/IsIterable.hpp"

namespace utils
{
template<typename Iterator>
struct RangePrinter
{
    RangePrinter(Iterator begin, Iterator end, const char* delimiter)
        : begin_{std::move(begin)}, end_{std::move(end)}, delimiter_{delimiter}
    {}

    friend std::ostream& operator<<(std::ostream& stream, const RangePrinter& printer)
    {
        auto begin = printer.begin_;

        stream << "[";
        if(begin != printer.end_)
        {
            stream << *begin;
            while(++begin != printer.end_)
            {
                stream << printer.delimiter_ << *begin;
            }
        }
        return stream << "]";
    }
private:
    Iterator begin_, end_;
    const char* delimiter_;
};

template<typename Range>
inline auto printRange(const Range& range, const char* delimiter = ", ")
{
    return RangePrinter{std::begin(range), std::end(range), delimiter};
}

template<typename Key, typename Value>
inline std::ostream& operator<<(std::ostream& os, const std::pair<Key, Value>& obj)
{
    return os << '{' << obj.first << ", " << obj.second << '}';
}

inline std::ostream& operator<<(std::ostream& os, const std::string& str)
{
    return os << str;
}

template<typename T, typename = std::enable_if_t<traits::is_iterable<T>>>
inline std::ostream& operator<<(std::ostream& os, const T& value)
{
    return os << printRange(value);
}
}