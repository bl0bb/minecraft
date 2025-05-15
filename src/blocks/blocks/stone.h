#ifndef _BLOCK_STONE_H
#define _BLOCK_STONE_H

#include "../blocks.h"

void _stone_init() {
    BLOCK_VOXEL_DATA[BlockTypes::STONE] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::STONE,
        BlockTextures::STONE,
        BlockTextures::STONE,
        BlockTextures::STONE,
        BlockTextures::STONE,
        BlockTextures::STONE,
    );
}

#endif