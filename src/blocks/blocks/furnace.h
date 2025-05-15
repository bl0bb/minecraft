#ifndef _BLOCK_FURNACE_H
#define _BLOCK_FURNACE_H

#include "../blocks.h"

constexpr inline RGBI4 getLightTest() {
    return Colors::createRGBI4(15, 0, 0, 15);
}

void _furnace_init() {
    BLOCK_VOXEL_DATA[BlockTypes::FURNACE] = BlockVoxelData(
        0,
        0,
        false,
        false,
        true,
        false,
        getLightTest,
        BlockTextures::FURNACE_TOP,
        BlockTextures::FURNACE_TOP,
        BlockTextures::FURNACE_SIDE,
        BlockTextures::FURNACE_SIDE,
        BlockTextures::FURNACE_FRONT,
        BlockTextures::FURNACE_FRONT,
    );
}

#endif