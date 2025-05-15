#ifndef _BLOCK_SNOW_H
#define _BLOCK_SNOW_H

#include "../blocks.h"

void _snow_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SNOW] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::SNOW,
        BlockTextures::SNOW,
        BlockTextures::SNOW,
        BlockTextures::SNOW,
        BlockTextures::SNOW,
        BlockTextures::SNOW,
    );
}

#endif