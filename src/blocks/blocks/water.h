#ifndef _BLOCK_WATER_H
#define _BLOCK_WATER_H

#include "../blocks.h"

constexpr inline BlockTexture _water_get_texture(const BlockStateStruct& state, u8 dir) {
    return BlockTextures::WATER;
}

void _water_init() {
    BLOCK_VOXEL_DATA[BlockTypes::WATER] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _water_get_texture
    );
}

#endif