#ifndef _BLOCK_OAK_PLANKS_H
#define _BLOCK_OAK_PLANKS_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_planks_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::OAK_PLANKS;
}

void _oak_planks_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_PLANKS] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _oak_planks_get_texture
    );
}

#endif