#ifndef _BLOCK_OAK_LOG_H
#define _BLOCK_OAK_LOG_H

#include "../blocks.h"

void _oak_log_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_LOG] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::OAK_LOG_TOP,
        BlockTextures::OAK_LOG_TOP,
        BlockTextures::OAK_LOG,
        BlockTextures::OAK_LOG,
        BlockTextures::OAK_LOG,
        BlockTextures::OAK_LOG,
    );
}

#endif