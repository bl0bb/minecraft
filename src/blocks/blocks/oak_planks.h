#ifndef _BLOCK_OAK_PLANKS_H
#define _BLOCK_OAK_PLANKS_H

#include "../blocks.h"

void _oak_planks_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_PLANKS] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::OAK_PLANKS,
        BlockTextures::OAK_PLANKS,
        BlockTextures::OAK_PLANKS,
        BlockTextures::OAK_PLANKS,
        BlockTextures::OAK_PLANKS,
        BlockTextures::OAK_PLANKS
    );
}

#endif