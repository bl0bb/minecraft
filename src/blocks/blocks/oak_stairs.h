#ifndef _BLOCK_OAK_STAIRS_H
#define _BLOCK_OAK_STAIRS_H

#include "../blocks.h"

void _oak_stairs_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_STAIRS] = BlockVoxelData(
        2,
        2,
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
        BlockTextures::OAK_PLANKS,
    );
}

#endif