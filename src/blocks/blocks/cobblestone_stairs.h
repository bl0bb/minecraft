#ifndef _BLOCK_COBBLESTONE_STAIRS_H
#define _BLOCK_COBBLESTONE_STAIRS_H

#include "../blocks.h"

constexpr inline BlockTexture _cobblestone_stairs_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::COBBLESTONE;
}

void _cobblestone_stairs_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE_STAIRS] = BlockVoxelData(
        BlockStateTypes::STAIR,
        BlockMeshTypes::STAIR,
        false,
        false,
        false,
        false,
        nullptr,
        _cobblestone_stairs_get_texture
    );
}

#endif