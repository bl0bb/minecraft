#ifndef _BLOCK_COBBLESTONE_SLAB_H
#define _BLOCK_COBBLESTONE_SLAB_H

#include "../blocks.h"

constexpr inline BlockTexture _cobblestone_slab_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::COBBLESTONE;
}

void _cobblestone_slab_init();

#endif