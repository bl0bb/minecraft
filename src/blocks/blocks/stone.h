#ifndef _BLOCK_STONE_H
#define _BLOCK_STONE_H

#include "../blocks.h"

constexpr inline BlockTexture _stone_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::STONE;
}

void _stone_init();

#endif