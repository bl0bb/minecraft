#ifndef _BLOCK_AIR_H
#define _BLOCK_AIR_H

#include "../blocks.h"

constexpr inline BlockTexture _air_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::NONE;
}

void _air_init() {
    BLOCK_VOXEL_DATA[BlockTypes::AIR] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        true,
        false,
        false,
        false,
        nullptr,
        _air_get_texture
    );
}

#endif