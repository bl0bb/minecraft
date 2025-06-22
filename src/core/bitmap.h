#ifndef BITMAP_H
#define BITMAP_H

#include <stdlib.h>
#include "types.h"

#define BITMAP_SIZE_TO_BYTES(_sz) ((((_sz) / 64) + 1) * sizeof(u64))

struct Bitmap {
    // members
    u64 size;
    u8* data;

    // static
    static Bitmap createWithInit(u64 _size, u8 initVal) {
        return Bitmap(_size, (u8*)(calloc(initVal, BITMAP_SIZE_TO_BYTES(_size))));
    }

    // constructors
    Bitmap() {}
    Bitmap(u64 _size, u8* _data) : size(_size), data(_data) {}
    Bitmap(u64 _size) : size(_size), data((u8*)(malloc(BITMAP_SIZE_TO_BYTES(_size)))) {}


    // methods


    // memory
    inline void reAlloc(u64 _size) {
        size = _size;
        data = (u8*)realloc(data, BITMAP_SIZE_TO_BYTES(size));
    }

    // data getting / setting
    inline void set(u64 n) {
        data[n / 8] |= (1 << (n % 8));
    }

    inline bool get(u64 n) {
        return (data[n / 8] & (1 << (n % 8))) != 0;
    }

    inline void clr(u64 n) {
        data[n / 8] &= ~(1 << (n % 8));
    }


    // cleanup
    inline void destroy() {
        free(data);
    }
};

#endif