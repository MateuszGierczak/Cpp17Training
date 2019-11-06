#pragma once

#include <ostream>

template<typename T, typename U>
std::ostream& operator<<(std::ostream& stream, std::pair<T, U> pair)
{
    return stream << "{" << pair.first << ", " << pair.second << "}";
}

namespace utils
{
    namespace detail
    {
        template<typename Iterator>
        struct range_printer
        {
            range_printer(Iterator begin, Iterator end)
                : begin_{begin},
                  end_{end}
            {}

            friend std::ostream& operator<<(std::ostream& stream, const range_printer& printer)
            {
                auto it = printer.begin_;

                stream << "{";

                if(it != printer.end_)
                {
                    stream << *it;
                    while(++it != printer.end_)
                    {
                        stream << ", " << *it;
                    }
                }

                return stream << "}";
            }
        private:
            Iterator begin_ {};
            Iterator end_ {};
        };
    }

    template<typename Iterator>
    inline auto print_range(Iterator begin, Iterator end)
    {
        return detail::range_printer<Iterator>{begin, end};
    }

    template<typename Range>
    inline auto print_range(const Range& range)
    {
        return print_range(std::begin(range), std::end(range));
    }
}
