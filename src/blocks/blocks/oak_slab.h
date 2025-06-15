#ifndef _BLOCK_OAK_SLAB_H
#define _BLOCK_OAK_SLAB_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_slab_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::OAK_PLANKS;
}

void _oak_slab_init();

#endif