#ifndef _BLOCK_SHORT_GRASS_H
#define _BLOCK_SHORT_GRASS_H

#include "../blocks.h"

constexpr inline BlockTexture _short_grass_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::SHORT_GRASS;
}

void _short_grass_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SHORT_GRASS] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::CROSS,
        false,
        false,
        false,
        false,
        nullptr,
        _short_grass_get_texture
    );
}

#endif