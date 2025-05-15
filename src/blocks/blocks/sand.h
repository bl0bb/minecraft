#ifndef _BLOCK_SAND_H
#define _BLOCK_SAND_H

#include "../blocks.h"

void _sand_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SAND] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::SAND,
        BlockTextures::SAND,
        BlockTextures::SAND,
        BlockTextures::SAND,
        BlockTextures::SAND,
        BlockTextures::SAND,
    );
}

#endif