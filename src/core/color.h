#ifndef COLOR_H
#define COLOR_H

#include "types.h"

// 0-3 (4) (16) r
// 4-7 (4) (16) g
// 8-11 (4) (16) b
// 12-15 (4) (16) intensity
typedef u16 RGBI4;

// 0-7 (8) (255) r
// 8-15 (8) (255) g
// 16-23 (8) (255) b
// 24-31 (8) (255) intensity
typedef u32 RGBI8;

namespace Colors {
    inline RGBI4 createRGBI4(u8 r, u8 g, u8 b, u8 i) {
        return (r << 12) | (g << 8) | (b << 4) | i;
    }

    inline RGBI8 createRGBI8(u8 r, u8 g, u8 b, u8 i) {
        return (r << 24) | (g << 16) | (b << 8) | i;
    }

    template<typename T>
    inline T reduceChannel(T channel) {
        return channel ? channel - 1 : channel;
    }

    inline RGBI8 reduceOne(RGBI8 color) {
        return (reduceChannel(color >> 12 & 0b1111) << 12) |
               (reduceChannel(color >> 8 & 0b1111) << 8) |
               (reduceChannel(color >> 4 & 0b1111) << 4) |
               reduceChannel(color & 0b1111);
    }
};

#endif