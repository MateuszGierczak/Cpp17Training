#pragma once

#include <ostream>
#include "traits/IsIterable.hpp"

namespace utils
{
template<typename T>
struct ValuePrinter
{
    const T& value;
};

template<typename T>
inline auto makeValuePrinter(const T& value)
{
    return ValuePrinter<T>{value};
}

template<typename Iterator>
struct RangePrinter
{
    RangePrinter(Iterator begin, Iterator end, const char* delimiter)
        : begin_{std::move(begin)}, end_{std::move(end)}, delimiter_{delimiter}
    {}

    friend std::ostream& operator<<(std::ostream& stream, const RangePrinter& printer)
    {
        auto begin = printer.begin_;

        stream << '[';
        if(begin != printer.end_)
        {
            stream << makeValuePrinter(*begin);
            while(++begin != printer.end_)
            {
                stream << printer.delimiter_ << makeValuePrinter(*begin);
            }
        }
        return stream << ']';
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
inline std::ostream& operator<<(std::ostream& os, const ValuePrinter<std::pair<Key, Value>>& obj)
{
    return os << '{' << makeValuePrinter(obj.value.first) << ", " << makeValuePrinter(obj.value.second) << '}';
}

inline std::ostream& operator<<(std::ostream& os, const ValuePrinter<std::string>& obj)
{
    return os << obj.value;
}

template<typename T, typename std::enable_if_t<traits::is_iterable<T>, int> = 0>
inline std::ostream& operator<<(std::ostream& os, const ValuePrinter<T>& obj)
{
    return os << printRange(obj.value);
}

template<typename T, typename std::enable_if_t<not traits::is_iterable<T>, int> = 0>
inline std::ostream& operator<<(std::ostream& os, const ValuePrinter<T>& obj)
{
    return os << obj.value;
}
}