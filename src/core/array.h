#ifndef _ARRAY_H
#define _ARRAY_H

#include <cstddef>

template<typename T, std::size_t N>
constexpr inline std::size_t array_size(T (&)[N]) noexcept {
    return N;
}

#endif