#ifndef BITS_H
#define BITS_H

#include <intrin.h>

#include "types.h"

// Returns the least significant bit in a non-zero bitboard.
inline u64 lsb(u64 b) {
#if defined(__GNUC__)  // GCC, Clang, ICX

    return u64(__builtin_ctzll(b));

#elif defined(_MSC_VER)
    #ifdef _WIN64  // MSVC, WIN64

    unsigned long idx;
    _BitScanForward64(&idx, b);
    return u64(idx);

#else  // MSVC, WIN32
    unsigned long idx;

    if (b & 0xffffffff)
    {
        _BitScanForward(&idx, int32_t(b));
        return u64(idx);
    }
    else
    {
        _BitScanForward(&idx, int32_t(b >> 32));
        return u64(idx + 32);
    }
#endif
#else  // Compiler is neither GCC nor MSVC compatible
#error "Compiler not supported."
#endif
}
    
// Returns the most significant bit in a non-zero bitboard.
inline u64 msb(u64 b) {
#if defined(__GNUC__)  // GCC, Clang, ICX

    return u64(63 ^ __builtin_clzll(b));

#elif defined(_MSC_VER)
#ifdef _WIN64  // MSVC, WIN64

    unsigned long idx;
    _BitScanReverse64(&idx, b);
    return u64(idx);

#else  // MSVC, WIN32

    unsigned long idx;

    if (b >> 32)
    {
        _BitScanReverse(&idx, int32_t(b >> 32));
        return u64(idx + 32);
    }
    else
    {
        _BitScanReverse(&idx, int32_t(b));
        return u64(idx);
    }
#endif
#else  // Compiler is neither GCC nor MSVC compatible
#error "Compiler not supported."
#endif
}
    
inline u8 pop_lsb(u64& b) {
    const u64 s = lsb(b);
    b &= b - 1;
    return s;
}

inline u8 countTrailingOnes(u64 num) {
    return lsb(~num);
}

#endif