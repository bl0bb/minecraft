#ifndef _BLOCK_COBBLESTONE_STAIRS_H
#define _BLOCK_COBBLESTONE_STAIRS_H

#include "../blocks.h"

void _cobblestone_stairs_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE_STAIRS] = BlockVoxelData(
        2,
        2,
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