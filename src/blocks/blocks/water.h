#ifndef _BLOCK_WATER_H
#define _BLOCK_WATER_H

#include "../blocks.h"

constexpr inline BlockTexture _water_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::WATER;
}

void _water_init();

#endif