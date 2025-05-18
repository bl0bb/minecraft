#ifndef _BLOCK_COBBLESTONE_SLAB_H
#define _BLOCK_COBBLESTONE_SLAB_H

#include "../blocks.h"

constexpr inline BlockTexture _cobblestone_slab_get_texture(const BlockStateStruct& state, u8 dir) {
    return BlockTextures::COBBLESTONE;
}

void _cobblestone_slab_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE_SLAB] = BlockVoxelData(
        BlockStateTypes::SLAB,
        BlockMeshTypes::SLAB,
        false,
        false,
        false,
        false,
        nullptr,
        _cobblestone_slab_get_texture
    );
}

#endif