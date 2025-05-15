#ifndef _BLOCK_WATER_H
#define _BLOCK_WATER_H

#include "../blocks.h"

void _water_init() {
    BLOCK_VOXEL_DATA[BlockTypes::WATER] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::WATER,
        BlockTextures::WATER,
        BlockTextures::WATER,
        BlockTextures::WATER,
        BlockTextures::WATER,
        BlockTextures::WATER,
    );
}

#endif