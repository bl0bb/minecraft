#ifndef _BLOCK_POPPY_H
#define _BLOCK_POPPY_H

#include "../blocks.h"

constexpr inline BlockTexture _poppy_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::POPPY;
}

void _poppy_init();

#endif