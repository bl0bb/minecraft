#ifndef _BLOCK_GRASS_H
#define _BLOCK_GRASS_H

#include "../blocks.h"

void _grass_init() {
    BLOCK_VOXEL_DATA[BlockTypes::GRASS] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::GRASS_BLOCK_TOP,
        BlockTextures::DIRT,
        BlockTextures::GRASS_BLOCK_SIDE,
        BlockTextures::GRASS_BLOCK_SIDE,
        BlockTextures::GRASS_BLOCK_SIDE,
        BlockTextures::GRASS_BLOCK_SIDE
    );
}

#endif