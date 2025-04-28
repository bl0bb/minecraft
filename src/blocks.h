#ifndef BLOCKS_H
#define BLOCKS_H

#include <vector>

#include "core/types.h"

// 0-9 top
// 10-19 bottom
// 20-29 right
// 30-39 left
// 40-49 back
// 50-59 front
typedef u64 BlockVoxelData;

namespace BlockVoxelDatas {
    const inline BlockVoxelData create(u16 top, u16 bottom, u16 right, u16 left, u16 back, u16 front) {
        // bitshift 1 for now to prevent the code from thinking all bits being 0 is an empty block
        return ((BlockVoxelData)1 << 63) |
            ((BlockVoxelData)front << 50) |
            ((BlockVoxelData)back << 40) |
            ((BlockVoxelData)left << 30) |
            ((BlockVoxelData)right << 20) |
            ((BlockVoxelData)bottom << 10) |
            (BlockVoxelData)top;
    }
    const inline u16 get_face(BlockVoxelData block, u8 face) {
        return (block >> (face * 10)) & 0b1111111111;
    }
};

constexpr const char* block_textures[] = {
    "grass_block_top",          //  0
    "grass_block_side",         //  1
    "dirt",                     //  2
    "cobblestone",              //  3
    "oak_planks",               //  4
    "crafting_table_top",       //  5
    "crafting_table_front",     //  6
    "crafting_table_side",      //  7
    "furnace_top",              //  8
    "furnace_side",             //  9
    "furnace_front",            // 10
};

enum BlockType : u8 {
    GRASS           = 0,
    DIRT            = 1,
    COBBLESTONE     = 2,
    OAK_PLANKS      = 3,
    CRAFTING_TABLE  = 4,
    FURNACE         = 5,
};

const BlockVoxelData block_voxel_datas[] = {
    BlockVoxelDatas::create(0, 2, 1, 1, 1, 1),
    BlockVoxelDatas::create(2, 2, 2, 2, 2, 2),
    BlockVoxelDatas::create(3, 3, 3, 3, 3, 3),
    BlockVoxelDatas::create(4, 4, 4, 4, 4, 4),
    BlockVoxelDatas::create(5, 4, 7, 7, 6, 6),
    BlockVoxelDatas::create(8, 8, 10, 10, 7, 7),
};

#endif