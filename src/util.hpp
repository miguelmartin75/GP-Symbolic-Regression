#ifndef UTIL_HPP
#define UTIL_HPP

#include <algorithm>

namespace util
{
    template <class T, class Container>
    bool contains(Container& container, const T& obj)
    {
        return std::find(std::begin(container), std::end(container), obj) != std::end(container);
    }
}

#endif // UTIL_HPP
