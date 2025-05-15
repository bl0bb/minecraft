#ifndef _BLOCK_POPPY_H
#define _BLOCK_POPPY_H

#include "../blocks.h"

void _poppy_init() {
    BLOCK_VOXEL_DATA[BlockTypes::POPPY] = BlockVoxelData(
        0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::POPPY,
        BlockTextures::POPPY,
        BlockTextures::POPPY,
        BlockTextures::POPPY,
        BlockTextures::POPPY,
        BlockTextures::POPPY,
    );
}

#endif