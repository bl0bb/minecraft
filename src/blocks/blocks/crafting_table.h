#ifndef _BLOCK_CRAFTING_TABLE_H
#define _BLOCK_CRAFTING_TABLE_H

#include "../blocks.h"

constexpr inline BlockTexture _crafting_table_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    if (dir == 0) {
        return BlockTextures::CRAFTING_TABLE_SIDE;
    } else if (dir == 1) {
        return BlockTextures::CRAFTING_TABLE_SIDE;
    } else if (dir == 2) {
        return BlockTextures::CRAFTING_TABLE_TOP;
    } else if (dir == 3) {
        return BlockTextures::OAK_PLANKS;
    } else if (dir == 4) {
        return BlockTextures::CRAFTING_TABLE_FRONT;
    } else {
        return BlockTextures::CRAFTING_TABLE_FRONT;
    }
}

void _crafting_table_init() {
    BLOCK_VOXEL_DATA[BlockTypes::CRAFTING_TABLE] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _crafting_table_get_texture
    );
}

#endif