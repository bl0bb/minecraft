#ifndef _BLOCK_GRASS_H
#define _BLOCK_GRASS_H

#include "../blocks.h"

constexpr inline BlockTexture _grass_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    if (dir == 0) {
        return BlockTextures::GRASS_BLOCK_SIDE;
    } else if (dir == 1) {
        return BlockTextures::GRASS_BLOCK_SIDE;
    } else if (dir == 2) {
        return BlockTextures::GRASS_BLOCK_TOP;
    } else if (dir == 3) {
        return BlockTextures::DIRT;
    } else if (dir == 4) {
        return BlockTextures::GRASS_BLOCK_SIDE;
    } else {
        return BlockTextures::GRASS_BLOCK_SIDE;
    }
}

void _grass_init() {
    BLOCK_VOXEL_DATA[BlockTypes::GRASS] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _grass_get_texture
    );
}

#endif