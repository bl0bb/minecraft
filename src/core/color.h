#ifndef COLOR_H
#define COLOR_H

#include "types.h"

// 0-3 (4) (16) r
// 4-7 (4) (16) g
// 8-11 (4) (16) b
typedef u16 RGB4;

// 0-3 (4) (16) r
// 4-7 (4) (16) g
// 8-11 (4) (16) b
// 12-15 (4) (16) intensity
typedef u16 RGBI4;

// 0-3 (4) (16) r
// 4-7 (4) (16) g
// 8-11 (4) (16) b
// 12-15 (4) (16) intensity
// 16-19 (4) (16) sun intensity
typedef u32 RGBIS4;

// 0-7 (8) (255) r
// 8-15 (8) (255) g
// 16-23 (8) (255) b
// 24-31 (8) (255) intensity
typedef u32 RGBI8;

namespace Colors {
    constexpr u8 COLOR4_MAX = 15;
    constexpr u8 COLOR8_MAX = 255;

    constexpr inline RGB4 createRGB4(u8 r, u8 g, u8 b) {
        return (b << 8) | (g << 4) | r;
    }

    constexpr inline RGBI4 createRGBI4(u8 r, u8 g, u8 b, u8 i) {
        return (i << 12) | (b << 8) | (g << 4) | r;
    }

    constexpr inline RGBIS4 createRGBIS4(u8 r, u8 g, u8 b, u8 i, u8 s) {
        return (s << 16) | (i << 12) | (b << 8) | (g << 4) | r;
    }

    constexpr inline RGBI8 createRGBI8(u8 r, u8 g, u8 b, u8 i) {
        return (i << 24) | (b << 16) | (g << 8) | r;
    }

    template<typename T>
    constexpr inline T reduceChannel(T channel) {
        return channel ? channel - 1 : channel;
    }

    constexpr inline RGBI4 reduceOne(RGBI4 color) {
        return (reduceChannel(color >> 12 & 0xF) << 12) |
               (reduceChannel(color >> 8 & 0xF) << 8) |
               (reduceChannel(color >> 4 & 0xF) << 4) |
               reduceChannel(color & 0xF);
    }

    constexpr inline RGBI8 reduceOne(RGBI8 color) {
        return (reduceChannel(color >> 24 & 0xFF) << 24) |
               (reduceChannel(color >> 16 & 0xFF) << 16) |
               (reduceChannel(color >> 8 & 0xFF) << 8) |
               reduceChannel(color & 0xFF);
    }

    constexpr inline RGBIS4 setSunlight(RGBIS4 color, u8 sunlight) {
        // TODO: typecast COLOR4_MAX to u32 here? (maybe even sunlight as well?)
        return (color & ~(COLOR4_MAX << 16)) | (sunlight << 16);
    }
};

#endif