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
typedef u32 RGB8;

// 0-7 (8) (255) r
// 8-15 (8) (255) g
// 16-23 (8) (255) b
// 24-31 (8) (255) intensity
typedef u32 RGBI8;

namespace Colors {
    constexpr u8 COLOR4_MAX = 15;
    constexpr u8 COLOR8_MAX = 255;

    constexpr inline RGB4 createRGB4(u8 r, u8 g, u8 b) {
        return (u64(b) << 8) | (u64(g) << 4) | u64(r);
    }

    constexpr inline RGB4 createRGB4(u8 val) {
        return createRGB4(val, val, val);
    }

    constexpr inline RGBI4 createRGBI4(u8 r, u8 g, u8 b, u8 i) {
        return (u64(i) << 12) | (u64(b) << 8) | (u64(g) << 4) | u64(r);
    }

    constexpr inline RGBI4 createRGBI4(u8 val) {
        return createRGBI4(val, val, val, val);
    }

    constexpr inline RGBIS4 createRGBIS4(u8 r, u8 g, u8 b, u8 i, u8 s) {
        return (u64(s) << 16) | (u64(i) << 12) | (u64(b) << 8) | (u64(g) << 4) | u64(r);
    }

    constexpr inline RGBIS4 createRGBIS4(u8 val) {
        return createRGBIS4(val, val, val, val, val);
    }

    constexpr inline RGB8 createRGB8(u8 r, u8 g, u8 b) {
        return (u64(b) << 16) | (u64(g) << 8) | u64(r);
    }

    constexpr inline RGB8 createRGB8(u8 val) {
        return createRGB8(val, val, val);
    }

    constexpr inline RGBI8 createRGBI8(u8 r, u8 g, u8 b, u8 i) {
        return (u64(i) << 24) | (u64(b) << 16) | (u64(g) << 8) | u64(r);
    }

    constexpr inline RGBI8 createRGBI8(u8 val) {
        return createRGBI8(val, val, val, val);
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
        return (color & ~(u64(COLOR4_MAX) << 16)) | (u64(sunlight) << 16);
    }
};

#endif