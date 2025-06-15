#ifndef _BLOCK_COBBLESTONE_H
#define _BLOCK_COBBLESTONE_H

#include "../blocks.h"

constexpr inline BlockTexture _cobblestone_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::COBBLESTONE;
}

void _cobblestone_init();

#endif