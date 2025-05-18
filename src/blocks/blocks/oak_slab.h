#ifndef _BLOCK_OAK_SLAB_H
#define _BLOCK_OAK_SLAB_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_slab_get_texture(const BlockStateStruct& state, u8 dir) {
    return BlockTextures::OAK_PLANKS;
}

void _oak_slab_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_SLAB] = BlockVoxelData(
        BlockStateTypes::SLAB,
        BlockMeshTypes::SLAB,
        false,
        false,
        false,
        false,
        nullptr,
        _oak_slab_get_texture
    );
}

#endif