#ifndef _BLOCK_SNOW_H
#define _BLOCK_SNOW_H

#include "../blocks.h"

constexpr inline BlockTexture _snow_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::SNOW;
}

void _snow_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SNOW] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _snow_get_texture
    );
}

#endif