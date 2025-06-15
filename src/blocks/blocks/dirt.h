#ifndef _BLOCK_DIRT_H
#define _BLOCK_DIRT_H

#include "../blocks.h"

constexpr inline BlockTexture _dirt_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::DIRT;
}

void _dirt_init();

#endif