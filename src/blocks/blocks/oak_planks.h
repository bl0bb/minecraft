#ifndef _BLOCK_OAK_PLANKS_H
#define _BLOCK_OAK_PLANKS_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_planks_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::OAK_PLANKS;
}

void _oak_planks_init();

#endif