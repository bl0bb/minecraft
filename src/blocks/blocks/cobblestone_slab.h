#ifndef _BLOCK_COBBLESTONE_SLAB_H
#define _BLOCK_COBBLESTONE_SLAB_H

#include "../blocks.h"

void _cobblestone_slab_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE_SLAB] = BlockVoxelData(
        1,
        1,
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