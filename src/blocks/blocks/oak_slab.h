#ifndef _BLOCK_OAK_SLAB_H
#define _BLOCK_OAK_SLAB_H

#include "../blocks.h"

void _oak_slab_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_SLAB] = BlockVoxelData(
        1,
        1,
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