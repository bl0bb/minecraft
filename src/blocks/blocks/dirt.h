#ifndef _BLOCK_DIRT_H
#define _BLOCK_DIRT_H

#include "../blocks.h"

constexpr inline BlockTexture _dirt_get_texture(const BlockStateStruct& state, u8 dir) {
    return BlockTextures::DIRT;
}

void _dirt_init() {
    BLOCK_VOXEL_DATA[BlockTypes::DIRT] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _dirt_get_texture
    );
}

#endif