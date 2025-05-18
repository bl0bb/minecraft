#ifndef _BLOCK_STONE_H
#define _BLOCK_STONE_H

#include "../blocks.h"

constexpr inline BlockTexture _stone_get_texture(const BlockStateStruct& state, u8 dir) {
    return BlockTextures::STONE;
}

void _stone_init() {
    BLOCK_VOXEL_DATA[BlockTypes::STONE] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _stone_get_texture
    );
}

#endif