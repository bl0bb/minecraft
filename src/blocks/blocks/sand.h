#ifndef _BLOCK_SAND_H
#define _BLOCK_SAND_H

#include "../blocks.h"

constexpr inline BlockTexture _sand_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::SAND;
}

void _sand_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SAND] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _sand_get_texture
    );
}

#endif