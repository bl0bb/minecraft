#ifndef _BLOCK_OAK_STAIRS_H
#define _BLOCK_OAK_STAIRS_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_stairs_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::OAK_PLANKS;
}

void _oak_stairs_init();

#endif