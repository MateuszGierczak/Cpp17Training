#pragma once

#include <ostream>

namespace utils
{
template<typename Iterator>
struct RangePrinter
{
    RangePrinter(Iterator begin, Iterator end, const char* delimiter)
        : begin_{begin}, end_{end}, delimiter_{delimiter}
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

template<typename Iterator>
inline std::ostream& operator<<(std::ostream& os, const RangePrinter<Iterator>& printer)
{
    return os << printer;
}

template<typename Key, typename Value>
inline std::ostream& operator<<(std::ostream& os, const std::pair<Key, Value>& obj)
{
    return os << '{' << obj.first << ", " << obj.second << '}';
}
}