#ifndef _BLOCK_OAK_LEAVES_H
#define _BLOCK_OAK_LEAVES_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_leaves_get_texture(const BlockStateStruct& state, u8 dir) {
    return BlockTextures::OAK_LEAVES;
}

void _oak_leaves_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_LEAVES] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        true,
        false,
        false,
        false,
        nullptr,
        _oak_leaves_get_texture
    );
}

#endif