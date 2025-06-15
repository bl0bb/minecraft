#ifndef _BLOCK_FURNACE_H
#define _BLOCK_FURNACE_H

#include "../blocks.h"

constexpr inline BlockTexture _furnace_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    if (dir == 0) {
        return BlockTextures::FURNACE_SIDE;
    } else if (dir == 1) {
        return BlockTextures::FURNACE_SIDE;
    } else if (dir == 2) {
        return BlockTextures::FURNACE_TOP;
    } else if (dir == 3) {
        return BlockTextures::FURNACE_TOP;
    } else if (dir == 4) {
        return BlockTextures::FURNACE_FRONT;
    } else {
        return BlockTextures::FURNACE_FRONT;
    }
}

constexpr inline RGBI4 getFurnaceLight() {
    return Colors::createRGBI4(15, 0, 0, 15);
}

void _furnace_init();

#endif