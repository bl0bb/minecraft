#ifndef _BLOCK_OAK_LEAVES_H
#define _BLOCK_OAK_LEAVES_H

#include "../blocks.h"

void _oak_leaves_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_LEAVES] = BlockVoxelData(
        0,
        0,
        true,
        false,
        false,
        false,
        nullptr,
        BlockTextures::OAK_LEAVES,
        BlockTextures::OAK_LEAVES,
        BlockTextures::OAK_LEAVES,
        BlockTextures::OAK_LEAVES,
        BlockTextures::OAK_LEAVES,
        BlockTextures::OAK_LEAVES,
    );
}

#endif