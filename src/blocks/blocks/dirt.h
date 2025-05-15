#ifndef _BLOCK_DIRT_H
#define _BLOCK_DIRT_H

#include "../blocks.h"

void _dirt_init() {
    BLOCK_VOXEL_DATA[BlockTypes::DIRT] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::DIRT,
        BlockTextures::DIRT,
        BlockTextures::DIRT,
        BlockTextures::DIRT,
        BlockTextures::DIRT,
        BlockTextures::DIRT,
    );
}

#endif