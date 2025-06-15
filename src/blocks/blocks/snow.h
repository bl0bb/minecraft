#ifndef _BLOCK_SNOW_H
#define _BLOCK_SNOW_H

#include "../blocks.h"

constexpr inline BlockTexture _snow_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::SNOW;
}

void _snow_init();

#endif