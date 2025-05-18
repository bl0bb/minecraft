#ifndef _BLOCK_OAK_STAIRS_H
#define _BLOCK_OAK_STAIRS_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_stairs_get_texture(const BlockStateStruct& state, u8 dir) {
    return BlockTextures::OAK_PLANKS;
}

void _oak_stairs_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_STAIRS] = BlockVoxelData(
        BlockStateTypes::STAIR,
        BlockMeshTypes::STAIR,
        false,
        false,
        false,
        false,
        nullptr,
        _oak_stairs_get_texture
    );
}

#endif