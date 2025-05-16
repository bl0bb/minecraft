#ifndef _BLOCK_CRAFTING_TABLE_H
#define _BLOCK_CRAFTING_TABLE_H

#include "../blocks.h"

void _crafting_table_init() {
    BLOCK_VOXEL_DATA[BlockTypes::CRAFTING_TABLE] = BlockVoxelData(0,
        0,
        false,
        false,
        false,
        false,
        nullptr,
        BlockTextures::CRAFTING_TABLE_TOP,
        BlockTextures::OAK_PLANKS,
        BlockTextures::CRAFTING_TABLE_SIDE,
        BlockTextures::CRAFTING_TABLE_SIDE,
        BlockTextures::CRAFTING_TABLE_FRONT,
        BlockTextures::CRAFTING_TABLE_FRONT
    );
}

#endif