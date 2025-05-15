#ifndef _BLOCK_COBBLESTONE_H
#define _BLOCK_COBBLESTONE_H

#include "../blocks.h"

void _cobblestone_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::COBBLESTONE,
        BlockTextures::COBBLESTONE,
        BlockTextures::COBBLESTONE,
        BlockTextures::COBBLESTONE,
        BlockTextures::COBBLESTONE,
        BlockTextures::COBBLESTONE,
    );
}

#endif