#ifndef _BLOCK_AIR_H
#define _BLOCK_AIR_H

#include "../blocks.h"

void _air_init() {
    BLOCK_VOXEL_DATA[BlockTypes::AIR] = BlockVoxelData(
        0,
        0,
        true,
        false,
        false,
        false,
        nullptr,
        BlockTextures::NONE,
        BlockTextures::NONE,
        BlockTextures::NONE,
        BlockTextures::NONE,
        BlockTextures::NONE,
        BlockTextures::NONE
    );
}

#endif